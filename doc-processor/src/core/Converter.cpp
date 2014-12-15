#include "StdAfx.h"
#include "Converter.h"
#include "../utils/FileFinder.h"
#include "../utils/Common.h"
#include "../automation/Selection.h"
#include "../automation/Range.h"
#include "../automation/WordApp.h"
#include "../utils/ScopedTimeCalculator.h"

#include <Poco/NumberFormatter.h>
#include <Poco/FileStream.h>
#include <Poco/String.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <sstream>
#include <fstream>

#include <T2embapi.h>
#include <boost/lexical_cast.hpp>

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
        0x2008,  /// "  Punctuation space"
        0x2013,  /// "– EN DASH"
        0x2014,  /// "— Em DASH"
        0x2018,  /// "‘ LEFT SINGLE QUOTATION MARK"
        0x2019,  /// "’ RIGHT SINGLE QUOTATION MARK"
        0x201C,  /// "“ LEFT DOUBLE QUATATION SIGN"
        0x201E,  /// "„ DOUBLE LOW-9 QUATATION SIGN"
        0x201F,  /// "‟ DOUBLE HIGH-RESERVED-9 QUATATION SIGN"
        0x201D,  /// "” RIGHT DOUBLE QUATATION SIGN"
        0x2022,
        0x2026   /// "… HORIZONTAL ELLIPSIS"
    };   

    int count = sizeof(identicalMappings) / sizeof(wchar_t);
    for (int i = 0; i < count; ++i)
        mapping_[identicalMappings[i]] = identicalMappings[i];
}

wchar_t CharMapping::lookUp( wchar_t ch, const string_t& fontName  )
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
        ss << "LANG: " << language_ << ", FONT: " << fontName 
            << ". No mapping for symbol: INTCODE (" << (int) ch << "), HEX (0x"
            << Poco::NumberFormatter::formatHex((int)ch, 4) << "), CHAR ("
            << toUtf8(wstring_t(1, ch)) << ")";
        logError(logger(), ss.str());
        it = mapping_.insert(std::make_pair(ch, ch)).first;
    }
    return it->second;
}

bool CharMapping::doConversion( const wstring_t& asciiText, wstring_t& unicodeText,
    const string_t& fontName )
{
    bool spacingOnly = true;

    unicodeText.resize(asciiText.size());
    for (size_t i = 0; i < asciiText.size(); ++i) {
        unicodeText[i] = lookUp(asciiText[i], fontName);
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
    if (mapFile.find("cm-arafi.txt") != string_t::npos) {
        /// clear default mapping
        for (wchar_t i = 0x0021; i <= 0x007E; ++i)
            quickMap_[i] = 0;
    }

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
        else {
            mapping_[from] = to;
        }

        /// add identical mapping to avoid false warnings
        mapping_[to] = to;
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
#ifdef SECURITY_ENABLED
    , security_(config)
#endif
{
    word_.reset(new WordApp());
    wordVisible_ = config->getBool("winword.visible", false);
    word_->setVisible(wordVisible_);

    logWarning(logger(), "Working in PRECISE MODE. GOOD");
    wantUtf8Text_ = config_->getBool("app.saveAlsoAsUTF8", false);

    initialize(
        config_->getString("input.folder", ""),
        config_->getString("output.folder", ""),
        config_->getString("mapping.folder", "")
    );

    specialChars_ += (wchar_t)1;     /// footnote
    specialChars_ += (wchar_t)2;     /// footnote
    specialChars_ += (wchar_t)7;     /// table indicators
    specialChars_ += (wchar_t)9;     /// 
    specialChars_ += (wchar_t)11;     /// 
    specialChars_ += (wchar_t)12;     /// 
    specialChars_ += (wchar_t)13;    /// paragraph ending
    specialChars_ += (wchar_t)21;    /// paragraph ending
    specialChars_ += (wchar_t)47;    /// 

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
            ScopedTimeCalculator stc(logger(), "Elapsed ");
            logInfo(logger(), "Processing [document]: " + *it);
            convertSingleDocPrecise(*it);
        }
        catch (const Poco::Exception& pe) {
            logError(logger(), pe.displayText());
        }
        catch (const std::exception& e) {
            logError(logger(), e.what());
        }
    }
}

string_t Converter::percentageStr( int current, int total )
{
    if (current > total)
        current = total;

    std::stringstream ss;
    ss <<  (int)(100.0 * (double) (current) / (double) total) << " % completed.";
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

        string_t::size_type p = font.find(',');
        if (p != string_t::npos) {
            logInfo(logger(), "Reused \'" + font.substr(0, p) + "\' for \'" + font + "\'");
            tCharMappingSp& reuseMapping = getCM(font.substr(0, p));
            fontCharMaps_.insert(std::make_pair(font, reuseMapping));
            return reuseMapping;
        }

        p = font.find("Bold");
        if (p != string_t::npos) {
            logInfo(logger(), "Reused \'" + font.substr(0, p) + "\' for \'" + font + "\'");
            tCharMappingSp& reuseMapping = getCM(font.substr(0, p));
            fontCharMaps_.insert(std::make_pair(font, reuseMapping));
            return reuseMapping;
        }

        p = font.find("Italic");
        if (p != string_t::npos) {
            logInfo(logger(), "Reused \'" + font.substr(0, p) + "\' for \'" + font + "\'");
            tCharMappingSp& reuseMapping = getCM(font.substr(0, p));
            fontCharMaps_.insert(std::make_pair(font, reuseMapping));
            return reuseMapping;
        }

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
    ss << "Fonts in document '" << name << "' are: ";
    for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it) {
        ss << *it << ", ";
    }
    logInfo(logger(), ss.str());
}

void Converter::convertSingleDocPrecise( const string_t& fileName )
{
#ifdef SECURITY_ENABLED
    if ( !security_.getKey().updateCounters(0) ) {
        logContent(security_.getKey());
        return;
    }
#endif

    tDocumentsSp docs = word_->getDocuments();
    tDocumentSp  doc  = docs->open(toUtf16(getInputAbsPath(fileName)));
    if (!doc) {
        logError(logger(), "Error while opening document: " + fileName);
        return;
    }

    /// -------------------------------------------///
    usedFonts_.clear();
    wstring_t docAsText;
//     tParagraphsSp paragraphs = doc->getParagraphs();
//     int count = paragraphs->getCount();

//     tSentencesSp sentences = doc->getSentences();
//     int sentCount = sentences->getCount();

//     for (int i = 1; i <= count; ++i) {
//         tParagraphSp p = paragraphs->getItem(i);
//         docAsText += processRangePrecise(p->getRange(), false);
//         std::cout << "\r" << percentageStr(i, count);
//     }

    tRangeSp r = doc->getContent();
    int64 totalBytes = r->getStoryLength();

    docAsText += processRangePreciseVer2(r, true);

    std::cout << std::endl;


    /// footnotes
    logInfo(logger(), "Processing [footnotes]: ");

    tFootnotesSp footnots = doc->getFootnotes();
    int notesCount = footnots->getCount();
    for (int i = 1; i <= notesCount; ++i) {
        tNoteSp note = footnots->getItem(i);
        tRangeSp r = note->getRange();
        processRangePreciseVer2(r, false);
        std::cout << "\r" << percentageStr(i, notesCount);
    }
    if (notesCount > 0)
        std::cout << std::endl;

    tSectionsSp sections = doc->getSections();
    int sectionsCount = sections->getCount();
    for (int i = 1; i <= sectionsCount; ++i) {
        tSectionSp section(new Section(sections->getItem(i)));

        tHeadersFootersSp hfs = section->getHeaders();
        if (hfs) {
            logInfo(logger(), "Processing [headers]: ");
            tHeaderFooterSp hf( new HeaderFooter(hfs->getItem(1)) );
            tRangeSp r = hf->getRange();
            processRangePreciseVer2(r, false);
        }

        hfs = section->getFooters();
        if (hfs) {
            logInfo(logger(), "Processing [footers]: ");
            tHeaderFooterSp hf( new HeaderFooter(hfs->getItem(1)) );
            tRangeSp r = hf->getRange();
            processRangePreciseVer2(r, false);
        }
    }

#ifdef SECURITY_ENABLED
    security_.getKey().updateCounters(totalBytes);
#endif

    logUsedFonts(fileName, usedFonts_);
    usedFonts_.clear();

    /// now save result in the appropriate folder
    string_t outputDir = getOutputAbsPath(fileName);
    Poco::File(outputDir).createDirectories();
    Poco::Path p(fileName);
    logInfo(logger(), "Saving document...");
    doc->saveAs( outputDir + p.getBaseName() + " UNICODE." + p.getExtension() );
    doc->close();
    logInfo(logger(), "Save was successful.");
    
    if ( wantUtf8Text_ )
        writeFileAsBinary( outputDir + p.getBaseName() + " UTF8.txt", toUtf8(docAsText));

#ifdef SECURITY_ENABLED
    logContent(security_.getKey());
#endif
}

void Converter::convertSingleDocQuick( const string_t& fileName )
{
    tDocumentsSp docs = word_->getDocuments();
    tDocumentSp  doc  = docs->open(toUtf16(getInputAbsPath(fileName)));
    if (!doc) {
        logError(logger(), "Error while opening document: " + fileName);
        return;
    }

    tCharMappingSp cm;
    string_t       fontName, newFontName;
    wstring_t      text, textUnicode, docAsText;
    int            c = 0;

    tSelectionSp s = word_->getSelection();
    int pos = 0;
    int totalCharsQty = s->getStoryLength();

    do {
        s->setStart(pos);
        s->setEnd(pos + 1);
        s->selectCurrentFont();
        fontName = s->getFont()->getName();

        if ( canSkipFont(fontName) ) {
            //s->getFont()->haveCommonAttributes();
            pos = s->getEnd();
            docAsText += s->getText();
            std::cout << "\r" << percentageStr(pos, totalCharsQty - 1);
            continue;
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
                pos = s->getEnd();
                docAsText += text;
                std::cout << "\r" << percentageStr(pos, totalCharsQty - 1);
                continue;
            }
        }

        /// use mapping
        textUnicode.clear();
        cm = getCM(fontName);
        if (cm) {
            bool spacingOnly = cm->doConversion(text, textUnicode, fontName);
            newFontName = getFontSubstitution(cm, fontName);
            //tFontSp fontDup = s->getFont()->duplicate();
            s->setText(textUnicode);
            //s->getFont()->haveCommonAttributes();
            s->getFont()->setName(newFontName);
            //s->setFont(fontDup);
        }

        /// extract text from the document as well
        docAsText += textUnicode;
        pos = s->getEnd();

        std::cout << "\r" << percentageStr(pos, totalCharsQty - 1);
    } while ( pos < totalCharsQty - 1 );


    /// -------------------------------------------///
    /// now save result in the appropriate folder  ///
    string_t outputDir = getOutputAbsPath(fileName);
    Poco::File(outputDir).createDirectories();
    Poco::Path p(fileName);
    doc->saveAs( outputDir + p.getBaseName() + " QUICK." + p.getExtension() );
    doc->close();

    if ( config_->getBool("app.saveAlsoAsUTF8", false) )
        writeFileAsBinary( outputDir + p.getBaseName() + " UTF8 QUICK.txt", toUtf8(docAsText));
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
/*
wstring_t Converter::processRangePrecise( tRangeSp& r, bool showProgress )
{
    string_t defaultFont = "Sylfaen";

    tCharMappingSp cm;
    string_t       fontName, newFontName;
    wstring_t      text, textUnicode, docAsText;
    
    int endLabel   = r->getEnd();
    int startLabel = r->getStart();
    int totalCharsQty = endLabel - startLabel;

    int pos = r->getStart();
    int endPos = 0;

    int CHUNK_SIZE = 256;
    if (totalCharsQty < CHUNK_SIZE)
        CHUNK_SIZE = totalCharsQty;

    do {
        int chunk = CHUNK_SIZE;
        r->setStart(pos);
        r->setEnd(pos + chunk);

        if (wordVisible_)
            r->select();

        /// try to find a text that can be skipped, that is a text with the
        /// Unicode font name
        tFontSp font = r->getFont();
        while (font->getName().empty() && chunk > 1) {
            chunk /= 2;
            r->setEnd(pos + chunk);
            if (wordVisible_) r->select();
            font = r->getFont();
        }

        if (canSkipFont(font->getName())) {
            if ( wantUtf8Text_ ) docAsText += r->getText();

            endPos = r->getEnd();
            if (endPos == pos)
                break;
            pos = endPos;
            if (showProgress)
                std::cout << "\r" << percentageStr(pos - startLabel, totalCharsQty - 1);
            continue;
        }

        /// skip invalid characters
        text     = r->getText();
        wstring_t::size_type xpos = text.find_first_of(specialChars_);
        if (xpos != wstring_t::npos) {
            if (xpos == 0) {
                if ( wantUtf8Text_ ) docAsText += text[xpos];
                ++pos;
                r->setEnd(pos);
                r->getFont()->setName(defaultFont);
                if (showProgress)
                    std::cout << "\r" << percentageStr(pos - startLabel, totalCharsQty - 1);
                continue;
            }
            chunk = xpos;
            r->setEnd(pos + chunk);
            if (wordVisible_) r->select();
        }

        /// we found a font that cannot be skipped, deal with it properly
        int tmpIndex = 0;
        while (!font->haveCommonAttributes(r, tmpIndex) && chunk > 1) {
            chunk /= 2;
            r->setEnd(pos + chunk);
            if (wordVisible_) r->select();
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

                font->setName(newFontName);

                if (!spaceOnly) {
                    font->setSize(font->getSize());
                    font->setBold(font->getBold());
                    font->setItalic(font->getItalic());
                    font->setUnderline(font->getUnderline());
                    font->setColor(font->getColor());
                    font->setUnderlineColor(font->getUnderlineColor());

                    r->setText(textUnicode);
                }
                if ( wantUtf8Text_ ) docAsText += textUnicode;
            }
        }
        else {
            if ( wantUtf8Text_ ) docAsText += text;
        }

        pos = r->getEnd();
        if (showProgress)
            std::cout << "\r" << percentageStr(pos - startLabel, totalCharsQty - 1);
    } while ( pos < endLabel - 1);

    return docAsText;
}

*/
wstring_t Converter::processRangePreciseVer2( tRangeSp& r, bool showProgress )
{
    static DWORD lastTicks = 0;

    wstring_t text, textUnicode;
    int pos      = r->getStart();
    int lastPos  = r->getEnd();
    int startPos = pos;
    int endPos   = pos;

    r->setRange(pos, pos);

    do {
        tRangeSp newRange = r->getNext(13, 1);
        if (!newRange) {
            newRange = r->getNext(2, 1);
            if (!newRange)
                break;
            r = newRange;
        }

        r = newRange;
        startPos = r->getStart();
        endPos   = r->getEnd();

        if (startPos > lastPos && pos > lastPos)
            break;

        r->setRange(pos, startPos);
        if (wordVisible_) 
            r->select();
        processRangeClassic2(r, text, textUnicode);

        r->setRange(startPos, endPos);
        if (wordVisible_) 
            r->select();
        processRangeClassic2(r, text, textUnicode);

        pos = endPos;
        if (showProgress) {
            DWORD ticks = GetTickCount();
            if (ticks - lastTicks > 30 || pos >= lastPos - 1) {
                std::cout << "\r" << percentageStr(pos, lastPos - 1);
                lastTicks = ticks;
            }
        }
    } while (true);
    return L"";
}

void Converter::processRangeClassic2( tRangeSp& r, wstring_t& text, wstring_t& textUnicode )
{
    static std::vector<int> offsets;

    tCharMappingSp cm;
    tFontSp font;
    string_t fontName, newFontName;

    font = r->getFont();
    fontName = font->getName();
    if (fontName.empty()) {
        logError(logger(), "empty font name");
        return;
    }

    usedFonts_.insert(fontName);

    if ( !canSkipFont(fontName) ) {
        cm = getCM(fontName);
        if (cm) {
            textUnicode.clear();
            text     = r->getText();
            cm->doConversion(text, textUnicode, fontName);
            newFontName = getFontSubstitution(cm, fontName);
            font->setName(newFontName);

            /// --------------------------------------------
            int enPos   = r->getEnd();
            int stPos = r->getStart();
            string_t::size_type off = 0;
            string_t::size_type pos = 0;

            offsets.clear();
            bool somethingChanged = false;
            while ( true )
            {
                pos = text.find_first_not_of(specialChars_, off);
                if (pos != string_t::npos) {
                    offsets.push_back(pos);
                    off = pos;
                    somethingChanged = true;
                }
                else {
                    break;
                }

                pos = text.find_first_of(specialChars_, off);
                if (pos != string_t::npos) {
                    offsets.push_back(pos);
                    off = pos + 1;
                    somethingChanged = true;
                }
                else {
                    offsets.push_back(text.size());
                    break;
                }
            }

            for (int i = 0; i < offsets.size(); i += 2) {
                int off1 = offsets[i];
                int off2 = offsets[i+1];
                r->setRange(stPos + off1, stPos + off2);
                if (wordVisible_) r->select();
                r->setText(textUnicode.substr(off1, off2 - off1));
            }

            if (somethingChanged)
                r->setRange(stPos, enPos);
        }
    }
}

void Converter::processRangeClassic( tRangeSp& r, wstring_t& text, wstring_t& textUnicode )
{
    tCharMappingSp cm;
    tFontSp font;
    string_t fontName, newFontName;

    font = r->getFont();
    fontName = font->getName();
    if (fontName.empty()) {
        logError(logger(), "empty font name");
        return;
    }

    if ( !canSkipFont(fontName) ) {
        cm = getCM(fontName);
        if (cm) {
            textUnicode.clear();
            text     = r->getText();
            cm->doConversion(text, textUnicode, fontName);
            newFontName = getFontSubstitution(cm, fontName);
            font->setName(newFontName);

            if (text.empty())
                return;
            else if ( text.size() == 1 ) {
                if (specialChars_.find(text[0]) != string_t::npos) {
                    return;
                }
                else if (text[0] <= 20 ) {
                    logWarning(logger(), boost::lexical_cast<string_t>((int) text[0]));
                    return;
                } 
            }

            tParagraphFormatSp pf = r->getParagraphFormat();
            int alignment = pf->getAlignment();
            float lineSpacing = pf->getLineSpacing();

            r->setText(textUnicode);
            
            if ( textUnicode.size() > 1 ) {
                pf->setAlignment(alignment);
                pf->setLineSpacing(lineSpacing);
            }
        }
    }
}


void Converter::processRangeHelper( tRangeSp& r, wstring_t& text, wstring_t& textUnicode, int pos )
{
    int st = r->getStart();
    int en = r->getEnd();

    text = r->getText();
    wstring_t::size_type xpos = text.find_first_of(specialChars_);
    if (xpos != wstring_t::npos) {
        if (xpos > 0) {
            r->setRange(st, st + xpos);
            if (wordVisible_) r->select();
            processRangeClassic(r, text, textUnicode);

            if ( en - (st + xpos + 1) > 0 ) {
                r->setRange(st + xpos + 1, en);
                if (wordVisible_) r->select();
                processRangeHelper(r, text, textUnicode, 0);
            }
        }
    }
    else {
        processRangeClassic(r, text, textUnicode);
    }   
}

