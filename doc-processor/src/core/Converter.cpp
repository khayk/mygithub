#include "StdAfx.h"
#include "Converter.h"
#include "../utils/FileFinder.h"
#include "../utils/Common.h"
#include "../automation/Selection.h"
#include "../automation/Range.h"
#include "../automation/WordApp.h"

#include <Poco/FileStream.h>
#include <Poco/String.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <sstream>
#include <fstream>

#include <T2embapi.h>


CharMapping::CharMapping()
    : LogSource("charMapping")
{
}

CharMapping::CharMapping( const string_t& mapFile )
    : LogSource("charMapping")
{
    memset(quickMap_, 0, sizeof(quickMap_));
    initDefaultMappings();
    loadMappingFile(mapFile);
}

CharMapping::~CharMapping()
{
}

void CharMapping::initDefaultMappings()
{
    for (wchar_t i = 0; i <= 0x007E; ++i)
        quickMap_[i] = i;

    wchar_t identicalMappings[] = {
        0x2018,
        0x2019,
        0x201C,
        0x201D,
        0x2026
    };

    int count = sizeof(identicalMappings) / sizeof(wchar_t);
    for (int i = 0; i < count; ++i)
        mapping_[identicalMappings[i]] = identicalMappings[i];
}

wchar_t CharMapping::lookUp( wchar_t ch )
{
    if (ch >= 0 && ch < 256) {
        /// this is very quick lookup if we are working with symbols with code
        /// < 256
        wchar_t val = quickMap_[ch];
        if (val != 0) 
            return val;

        /// no mapping in this range, look inside std::map
    }

    auto it = mapping_.find(ch);
    if (it == mapping_.end()) {
        std::stringstream ss;
        ss << "Language: " << language_ 
            << ". No mapping for symbol: INTCODE (" << (int) ch << ") CHAR ("
            << toUtf8(wstring_t(1, ch)) << ")";
        logError(logger(), ss.str());
        it = mapping_.insert(std::make_pair(ch, ch)).first;
    }
    return it->second;
}

bool CharMapping::doConversion( const wstring_t& asciiText, wstring_t& unicodeText )
{
    bool spacingOnly = true;

    unicodeText.resize(asciiText.size());
    for (size_t i = 0; i < asciiText.size(); ++i) {
        unicodeText[i] = lookUp(asciiText[i]);
        spacingOnly = (spacingOnly && 
            (iswspace(unicodeText[i]) || unicodeText[i] == 1));
    }
    return spacingOnly;
}

void CharMapping::updateCharMapping( const string_t& mapFile )
{
    loadMappingFile(mapFile);
}

void CharMapping::loadMappingFile( const string_t& mapFile )
{
    Poco::FileInputStream fis(mapFile);
    
    wchar_t from, to;
    while (!fis.eof()) {
        string_t sl;
        wstring_t wsl;
        std::getline(fis, sl);

        sl = Poco::trim(sl);
        if (sl.empty())
            continue;

        wsl = toUtf16(sl);
        if (wsl.size() != 3)
            throw std::logic_error("Invalid character mapping file: " + mapFile);
        from = wsl[0];
        to   = wsl[2];

        if (from < 256)
            quickMap_[from] = to;
        else
            mapping_[from] = to;
    }
}

string_t CharMapping::getLanguage() const
{
    return language_;
}

void CharMapping::setLanguage( const string_t& language )
{
    language_ = language;
}


/// ----------------------------------------------------------------------------
///                               CONVERTER
/// ----------------------------------------------------------------------------
Converter::Converter(const tConfigPtr& config)
    : LogSource("converter")
    , config_(config)
    , quickMode_(true)
{
    word_.reset(new WordApp());
    word_->setVisible(config->getBool("winword.visible", false));

    if ( config->getString("working.mode", "") == "precise" )
        quickMode_ = false;

    initialize(
        config_->getString("input.folder", ""),
        config_->getString("output.folder", ""),
        config_->getString("mapping.folder", "")
    );
}


Converter::~Converter()
{
    word_.reset();
}

void Converter::initialize( 
    const string_t& inputFolder, 
    const string_t& outputFolder,
    const string_t& mappingFolder )
{
    inputFolder_   = Poco::Path(inputFolder)
        .makeAbsolute()
        .makeDirectory()
        .toString();

    outputFolder_  = Poco::Path(outputFolder)
        .makeAbsolute()
        .makeDirectory()
        .toString();

    mappingFolder_ = Poco::Path(mappingFolder)
        .makeAbsolute()
        .makeDirectory()
        .toString();

    /// load font names which support all languages. these are generic unicode
    loadFontList(mappingFolder_ + "font-generic-unicode.txt", fontsAllCharSupport_);

    /// load fonts that should be ignore as they are discovered
    loadFontList(mappingFolder_ + "font-ignore-list.txt",     fontsToIgnore_);
    
    /// characters mapping for each available language
    FileFinder ff(true, mappingFolder_);
    auto files = ff.getFiles();
    auto dirs  = ff.getDirs();

    for (auto itd = dirs.begin(); itd != dirs.end(); ++itd) {
        Poco::Path langDir = Poco::Path(ff.getRootPath())
            .pushDirectory(*itd)
            .makeDirectory();

        loadKnownAsciiFonts(langDir.toString(), *itd);
        loadNamesMapping(langDir.toString() + "font-mapping-default.txt", *itd, false);
        loadNamesMapping(langDir.toString() + "font-mapping-user.txt", *itd, true);
    }
}


void Converter::loadKnownAsciiFonts( const string_t& languageDir,
    const string_t& language)
{
    string_t fontsKnow = languageDir + "font-known-ascii.txt";
    string_t charDefMapping = languageDir + "char-mapping-default.txt";

    tCharMappingSp defMapping(new CharMapping(charDefMapping));
    defMapping->setLanguage(language);

    /// open known fonts file and create mapping for each font
    Poco::FileInputStream fis(fontsKnow);

    while (!fis.eof()) {
        string_t sl;
        string_t fontName;
        string_t customMapFile;

        std::getline(fis, sl);

        /// check if there is custom mapping file specified current font
        string_t::size_type p = sl.find('|');
        fontName = sl.substr(0, p);
        sl = Poco::trim(sl);

        if (p != string_t::npos)
            customMapFile = sl.substr(p + 1);

        fontName = Poco::trimRight(fontName);
        customMapFile = Poco::trimLeft(customMapFile);

        if (fontName.empty())
            continue;

        if (customMapFile.empty()) {
            fontCharMaps_.insert( std::make_pair(fontName,
                defMapping) );
        }
        else {
            tCharMappingSp cm(new CharMapping);
            *cm = *defMapping;
            cm->updateCharMapping(languageDir + customMapFile);
            fontCharMaps_[fontName] = cm;
        }
    }
}

void Converter::loadNamesMapping( 
    const string_t& fileName, 
    const string_t& language, 
    bool optional )
{
    if ( optional ) {
        if ( !Poco::File(fileName).exists() ) {
            logInfo(logger(), "Files does not exists: " + fileName);
            return;
        }
    }

    Poco::FileInputStream fis(fileName);

    while (!fis.eof()) {
        string_t singleLine;
        std::getline(fis, singleLine);

        singleLine = Poco::trim(singleLine);
        if (singleLine.empty())
            continue;

        string_t::size_type pos = singleLine.find('|');
        string_t from = singleLine.substr(0, pos);
        string_t to   = singleLine.substr(pos + 1);

        from = Poco::trimRight(from);
        to   = Poco::trimLeft(to);

        if (from == "Default") {
            defaultFonts_[language] = to;
            continue;
        }

        fontNameMap_[from] = to;
    }
}

void Converter::loadFontList( const string_t& fileName, std::set<string_t>& cnt )
{
    /// fonts
    Poco::FileInputStream fis(fileName);
    while (!fis.eof()) {
        string_t fontName;
        std::getline(fis, fontName);

        fontName = Poco::trim(fontName);
        if (fontName.empty())
            continue;
        cnt.insert( fontName );
    }
}

bool Converter::isUnicodeFont( const string_t& name ) const
{
    return fontsAllCharSupport_.find(name) != fontsAllCharSupport_.end();
}

bool Converter::isIgnoredFont( const string_t& name ) const
{
    return fontsToIgnore_.find(name) != fontsToIgnore_.end();
}

void Converter::start()
{
    FileFinder ff(true, inputFolder_, "docx;doc");

    auto files = ff.getFiles();
    for (auto it = files.begin(); it != files.end(); ++it) {
        try {
            logInfo(logger(), "Processing document: " + *it);
            convertSingleDocQuick(*it);
        }
        catch (const Poco::Exception& pe) {
            logError(logger(), pe.displayText());
        }
        catch (const std::exception& e) {
            logError(logger(), e.what());
        }
    }
}

void Converter::convertSingleDoc( const string_t& fileName )
{
    string_t fullPath = inputFolder_ + fileName;

    logInfo(logger(), "Processing document: " + fullPath);
    Poco::Path p(fullPath);
    p.makeAbsolute();
    wstring_t wideName = toUtf16(p.toString());

    tDocumentsSp docs = word_->getDocuments();
    tDocumentSp  doc  = docs->open(wideName);
    if (!doc) {
        logError(logger(), "Error while opening document: " + fullPath);
        return;
    }

    tCharMappingSp cm;
    string_t       fontName, newFontName;
    wstring_t      text, textUnicode, docAsText;
    int            c = 0;
    

    //tParagraphsSp pars = doc->getParagraphs();

    //tParagraphSp itr = pars->getFirst();
    tCharactersSp chars = doc->getCharacters();
    int totalCharsQty = chars->getCount();
    tRangeSp itr = chars->getFirst();

    while (itr) {
        tRangeSp& r = itr;

        //r->select();
        text = r->getText();
        fontName = r->getFont()->getName();
        if (!fontName.empty()) {
            if ( !canSkipFont(fontName) ) {
                text = r->getText();
                cm = getCM(fontName);
                if (cm) {
                    textUnicode.clear();
                    bool spacingOnly = cm->doConversion(text, textUnicode);
                    newFontName = getFontSubstitution(cm, fontName);

                    if ( !spacingOnly ) {
                        //r->getFont()->reset();
                        int ci = r->getHighlightColorIndex();
                        r->setText(textUnicode);
                        r->autoFormat();
                    }
                    r->getFont()->setName(newFontName);
                }
            }
        }
        else {
            logError(logger(), "FOUND EMPTY FONT");
        }

        std::cout << "\r" << percentageStr(c, totalCharsQty);
        ++c;
        itr = itr->getNext();
    }
    //tRangeSp r = doc->getContent();
/*
    tSelectionSp r = word_->getSelection();
    int endPos = 0, startPos = 0;
    int totalCharsQty = r->getStoryLength();


    ///std::set<string_t> usedFonts;

    int CHUNK_SIZE = 256;
    if (totalCharsQty < CHUNK_SIZE)
        CHUNK_SIZE = totalCharsQty;

    int pos = 0;
    do {
        int chunk = CHUNK_SIZE;
        r->setStart(pos);
        r->setEnd(pos + chunk);
        //r->select();

        fontName = r->getFont()->getName();
        while (fontName.empty() && chunk > 1) {
            chunk /= 2;
            r->setEnd(pos + chunk);
            //r->select();
            fontName = r->getFont()->getName();
        }
        
        if ( !canSkipFont(fontName) ) {
            text = r->getText();
            cm = getCM(fontName);
            if (cm) {
                textUnicode.clear();
                cm->doConversion(text, textUnicode);
                substFont = getFontSubstitution(cm, fontName);

                r->setText(textUnicode);
                r->getFont()->setName(substFont);
            }
        }
        pos = r->getEnd();
        //r->setStart(r->getEnd());

/*        startPos = s->getStart();
        s->setStart(startPos + 1);
        //s->setEnd(startPos + 1);
        //s->selectCurrentFont();
        fontName = s->getFont()->getName();

        if ( canSkipFont(fontName) ) {
            s->getFont()->haveCommonAttributes();
            s->setStart(s->getEnd());
            docAsText += s->getText();
        }

        text = s->getText();
        if ( fontName.empty() ) {
            saveSelection(s);
            fontName = makeGuess(s);            
            restoreSelection(s);

            /// if after all we have empty font name, log about that event
            /// and go forward
            if (fontName.empty()) {
                logError(logger(), "EMPTY FONT NAME: Investigate");
                s->setStart(s->getEnd());
                continue;
            }
        }

        /// use mapping
        textUnicode.clear();
        cm = getCM(s, fontName);
        if (cm) {
            bool spacingOnly = cm->doConversion(text, textUnicode);
            substFont = getFontSubstitution(cm, fontName);
            //tFontSp fontDup = s->getFont()->duplicate();
            s->setText(textUnicode);
            //s->getFont()->haveCommonAttributes();
            s->getFont()->setName(substFont);
            //s->setFont(fontDup);
        }

        /// extract text from the document as well
        docAsText += textUnicode;
        std::cout << "\r" << percentageStr(endPos, totalCharsQty);
        s->setStart(s->getEnd());
    } while ( pos < totalCharsQty - 1 );*/

//     if ( !spacingOnly ) {
//         if (!quickMode_)   
//             s->copyFormat();
// 
//         s->setSelectionText(textUnicode);
// 
//         if (!quickMode_)   
//             s->pasteFormat();
//     }

    /// now save result in the appropriate folder
    string_t outputDir = Poco::Path(outputFolder_)
        .append( Poco::Path(fileName).parent() )
        .makeAbsolute()
        .makeDirectory()
        .toString();

    Poco::File(outputDir).createDirectories();
    doc->saveAs( outputDir + p.getBaseName() + "_Unicode." + p.getExtension() );
    doc->close();
    writeFileAsBinary( outputDir + p.getBaseName() + ".txt", toUtf8(docAsText));

    std::stringstream ss;
    ss << "Fonts found in the document '" << fullPath << "' are: ";
//     for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it) {
//         ss << "[" << *it << "] ";
//     }
    logInfo(logger(), ss.str());
}

string_t Converter::percentageStr( int current, int total )
{
    std::stringstream ss;
    ss <<  (int)(100.0 * (double) (current + 1) / (double) total) << " % completed.";
    return ss.str();
}

bool Converter::canSkipFont( const string_t& name ) const
{
    return (isUnicodeFont(name) || isIgnoredFont(name));
}

tCharMappingSp& Converter::getCM( const string_t& font )
{
    /// select mapping
    auto it = fontCharMaps_.find(font);
    if (it == fontCharMaps_.end()) {
        if ( !canSkipFont(font) ) {
            logError(logger(), "Font '" + font + "' is not found in mapping folder");

            /// create empty mapping to avoid repeated errors
            fontCharMaps_.insert(std::make_pair(font, noMapping));
        }
        return noMapping;
    }

    return it->second;
}

void Converter::saveSelection(tSelectionSp& s)
{
    savedStart_ = s->getStart();
    savedEnd_   = s->getEnd();
    hasSavedSelection_ = true;
}

void Converter::restoreSelection(tSelectionSp& s)
{
    if (hasSavedSelection_) {
        s->setStart(savedStart_);
        s->setEnd(savedEnd_);
        hasSavedSelection_ = false;
    }
}

void Converter::resetSavedSelection()
{
    hasSavedSelection_ = false;
}

string_t Converter::makeGuess( tSelectionSp& s )
{
    /// try to do guess
    /// select the font at the middle of the selected text
    int tmpStartPos = s->getStart();
    int tmpEndPos   = s->getEnd();
    int midPos = (tmpStartPos + tmpEndPos) / 2;
    s->setStart(midPos);
    s->moveCursor(mdRight, false);
    return s->getFont()->getName();
}

string_t Converter::getFontSubstitution( const tCharMappingSp& cm, const string_t& fontName )
{
    auto fit = fontNameMap_.find(fontName);
    if (fit == fontNameMap_.end()) {
        return defaultFonts_[cm->getLanguage()];
    }
    return fit->second;
}

void Converter::logUsedFonts( const string_t& name, std::set<string_t>& usedFonts )
{
    std::stringstream ss;
    ss << "Fonts found in the document '" << name << "' are: ";
    for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it) {
        ss << "[" << *it << "] ";
    }
    logInfo(logger(), ss.str());
}

void Converter::convertSingleDocQuick( const string_t& fileName )
{
    tDocumentsSp docs = word_->getDocuments();
    tDocumentSp  doc  = docs->open(toUtf16(getInputAbsPath(fileName)));
    if (!doc) {
        logError(logger(), "Error while opening document: " + fileName);
        return;
    }

    //tParagraphsSp  allParagraphs = doc->getParagraphs();
    //tParagraphSp   pgrph = allParagraphs->getFirst();
    //int            totalParagraphs = allParagraphs->getCount();

    usedFonts_.clear();
    wstring_t docAsText;

    docAsText += processRangeQuick(doc->getContent());

//     while (pgrph) {
//         tRangeSp r = pgrph->getRange();
//         pgrph = pgrph->getNext();
//     }

    /*


/*        startPos = s->getStart();
        s->setStart(startPos + 1);
        //s->setEnd(startPos + 1);
        //s->selectCurrentFont();
        fontName = s->getFont()->getName();

        if ( canSkipFont(fontName) ) {
            s->getFont()->haveCommonAttributes();
            s->setStart(s->getEnd());
            docAsText += s->getText();
        }

        text = s->getText();
        if ( fontName.empty() ) {
            saveSelection(s);
            fontName = makeGuess(s);            
            restoreSelection(s);

            /// if after all we have empty font name, log about that event
            /// and go forward
            if (fontName.empty()) {
                logError(logger(), "EMPTY FONT NAME: Investigate");
                s->setStart(s->getEnd());
                continue;
            }
        }

        /// use mapping
        textUnicode.clear();
        cm = getCM(s, fontName);
        if (cm) {
            bool spacingOnly = cm->doConversion(text, textUnicode);
            substFont = getFontSubstitution(cm, fontName);
            //tFontSp fontDup = s->getFont()->duplicate();
            s->setText(textUnicode);
            //s->getFont()->haveCommonAttributes();
            s->getFont()->setName(substFont);
            //s->setFont(fontDup);
        }

        /// extract text from the document as well
        docAsText += textUnicode;
        std::cout << "\r" << percentageStr(endPos, totalCharsQty);
        s->setStart(s->getEnd());
    */

//     if ( !spacingOnly ) {
//         if (!quickMode_)   
//             s->copyFormat();
// 
//         s->setSelectionText(textUnicode);
// 
//         if (!quickMode_)   
//             s->pasteFormat();
//     }

    /// now save result in the appropriate folder
    string_t outputDir = getOutputAbsPath(fileName);
    Poco::File(outputDir).createDirectories();
    Poco::Path p(fileName);
    doc->saveAs( outputDir + p.getBaseName() + " UNICODE." + p.getExtension() );
    doc->close();

    if ( config_->getBool("app.saveAlsoAsUTF8", false) )
        writeFileAsBinary( outputDir + p.getBaseName() + " UTF8.txt", toUtf8(docAsText));
}

string_t Converter::getInputAbsPath( const string_t& name )
{
    string_t fullPath = inputFolder_ + name;
    Poco::Path p(fullPath);
    p.makeAbsolute();
    return p.toString();
}

string_t Converter::getOutputAbsPath( const string_t& name )
{
    return Poco::Path(outputFolder_)
        .append( Poco::Path(name).parent() )
        .makeAbsolute()
        .makeDirectory()
        .toString();
}

wstring_t Converter::processRangeQuick( tRangeSp& r )
{
    string_t defaultFont = "Sylfaen";
    wstring_t       specialChars;
    specialChars += (wchar_t)2;     /// footnote
    specialChars += (wchar_t)7;     /// table indicators
    specialChars += (wchar_t)13;    /// paragraph ending

    tCharMappingSp cm;
    string_t       fontName, newFontName;
    wstring_t      text, textUnicode, docAsText;
    int            endPos = r->getEnd(), startPos = r->getStart();
    int            totalCharsQty = r->getStoryLength();

    int CHUNK_SIZE = 256;
    if (totalCharsQty < CHUNK_SIZE)
        CHUNK_SIZE = totalCharsQty;

    int pos = r->getStart();
    do {
        int chunk = CHUNK_SIZE;
        r->setStart(pos);
        r->setEnd(pos + chunk);
        r->select();

        /// try to find a text that can be skipped, that is a text with the
        /// unicode font name
        tFontSp font = r->getFont();
        while (font->getName().empty() && chunk > 1) {
            chunk /= 2;
            r->setEnd(pos + chunk);
            r->select();
            font = r->getFont();
        }
        if (canSkipFont(font->getName())) {
            docAsText += r->getText();
            pos = r->getEnd();
            continue;
        }

        /// skip invalid characters
        text     = r->getText();
        string_t::size_type xpos = text.find_first_of(specialChars);
        if (xpos != string_t::npos) {
            if (xpos == 0) {
                ++pos;
                r->setEnd(pos);
                r->getFont()->setName(defaultFont);
                continue;
            }
            chunk = xpos;
            r->setEnd(pos + chunk);
            r->select();
        }

        /// the font we found cannot be skipped, deal with it properly
        while (!font->haveCommonAttributes() && chunk > 1) {
            chunk /= 2;
            r->setEnd(pos + chunk);
            r->select();
            font = r->getFont();
        }

        fontName = font->getName();
        text     = r->getText();
        if ( !canSkipFont(fontName) ) {
            cm = getCM(fontName);
            if (cm) {
                textUnicode.clear();
                bool spaceOnly = cm->doConversion(text, textUnicode);
                newFontName = getFontSubstitution(cm, fontName);

                font = r->getFont();
                font->setName(newFontName);
                font->setSize(font->getSize());

                if (!spaceOnly)
                    r->setText(textUnicode);
                docAsText += textUnicode;
            }
        }
        else {
            docAsText += text;
        }

        pos = r->getEnd();

        std::cout << "\r" << percentageStr(pos, totalCharsQty - 1);
    } while ( pos < totalCharsQty - 1 );

    return docAsText;
}
