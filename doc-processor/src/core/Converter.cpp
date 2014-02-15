#include "StdAfx.h"
#include "Converter.h"
#include "../utils/FileFinder.h"
#include "../utils/Common.h"
#include "../automation/Selection.h"
#include "../automation/WordApp.h"

#include <Poco/FileStream.h>
#include <Poco/String.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <sstream>
#include <fstream>

#include <T2embapi.h>

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
        ss << "No mapping for symbol: CHAR (" << ch << ") INTCODE: ("
            << (int) ch << ")";
        logError(logger(), ss.str());
        it = mapping_.insert(std::make_pair(ch, ch)).first;
    }
    return it->second;
}

void CharMapping::doConversion( const wstring_t& asciiText, wstring_t& unicodeText )
{
    unicodeText.resize(asciiText.size());
    for (size_t i = 0; i < asciiText.size(); ++i) {
        unicodeText[i] = lookUp(asciiText[i]);
    }
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


/// ----------------------------------------------------------------------------
///                               CONVERTER
/// ----------------------------------------------------------------------------
Converter::Converter()
    : LogSource("converter")
{
    word_.reset(new WordApp());
    word_->setVisible(false);
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

        loadKnownAsciiFonts(langDir.toString());
        loadNamesMapping(langDir.toString() + "font-mapping-default.txt", false);
        loadNamesMapping(langDir.toString() + "font-mapping-user.txt", true);
    }
}


void Converter::loadKnownAsciiFonts( const string_t& languageDir )
{
    string_t fontsKnow = languageDir + "font-known-ascii.txt";
    string_t charDefMapping = languageDir + "char-mapping-default.txt";

    tCharMappingSp defMapping(new CharMapping(charDefMapping));

    /// open know fonts file and create mapping for each font
    Poco::FileInputStream fis(fontsKnow);

    while (!fis.eof()) {
        string_t fontName;
        std::getline(fis, fontName);

        fontName = Poco::trim(fontName);
        if (fontName.empty())
            continue;

        fontCharMaps_.insert( std::make_pair(fontName,
            defMapping) );
    }
}

void Converter::loadNamesMapping( const string_t& fileName, bool optional )
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

        if (singleLine.empty())
            break;

        string_t::size_type pos = singleLine.find('|');
        string_t from = singleLine.substr(0, pos);
        string_t to   = singleLine.substr(pos + 1);

        from = Poco::trim(from);
        to   = Poco::trim(to);

        if (from == "Default") {
            defaultFont_ = to;
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
    FileFinder ff(false, inputFolder_, "docx;doc");

    auto files = ff.getFiles();
    for (auto it = files.begin(); it != files.end(); ++it) {
        convertSingleDoc(ff.getRootPath() + *it);
    }
}

void Converter::convertSingleDoc( const string_t& fileName )
{
    logInfo(logger(), "Processing document: " + fileName);
    Poco::Path p(fileName);
    p.makeAbsolute();
    wstring_t wideName = toUtf16(p.toString());

    tDocumentsSp docs = word_->getDocuments();
    tDocumentSp doc   = docs->open(wideName);
    if (!doc) {
        logError(logger(), "Error while opening document: " + fileName);
        return;
    }

    tSelectionSp s = word_->getSelection();
    int endPos = 0, startPos = 0;
    int totalCharsQty = s->allCharactersCount();

    std::set<string_t> usedFonts;
    string_t docAsText;

    do {
        /// select current font
        s->selectCurrentFont();
        string_t fontName = s->getFont()->getFaceName();
        wstring_t text = s->getSelectionText(), textUnicode;

        startPos = s->getStartPos();
        endPos = s->getEndPos();

        bool restoreSelection = false;
        if (fontName.empty()) {
            /// try to do guess
            /// select the font at the middle of the selected text
            int tmpStartPos = s->getStartPos();
            int tmpEndPos   = s->getEndPos();
            int midPos = (tmpStartPos + tmpEndPos) / 2;
            s->setStartPos(midPos);
            s->moveCursor(Selection::mdRight, false);
            fontName = s->getFont()->getFaceName();
            restoreSelection = true;
        }

        tCharMappingSp cm;
        auto it = fontCharMaps_.find(fontName);
        if (it == fontCharMaps_.end()) {
            if ( !isUnicodeFont(fontName) && !isIgnoredFont(fontName) ) {
                logError(logger(), "Font '" + fontName + "' is not found in mapping folder");
                /// create empty mapping to avoid repeated errors
                it = fontCharMaps_.insert(std::make_pair(fontName, tCharMappingSp())).first;
            }
        }
        else
            cm = it->second;

        if (fontName.empty()) {
            logError(logger(), "EMPTY FONT NAME: Investigate");
        }
        usedFonts.insert(fontName);

        if (cm) {
            cm->doConversion(text, textUnicode);
            
            /// if we need to restore selection
            if (restoreSelection) {
                s->setStartPos(startPos);
                s->selectCurrentFont();
            }

            string_t newFaceName;
            auto fit = fontNameMap_.find(fontName);
            if (fit == fontNameMap_.end()) {
                newFaceName = defaultFont_;
            }
            else {
                newFaceName = fit->second;
            }

            /// extract text from the document as well
            docAsText += toUtf8(textUnicode);

            s->setSelectionText(textUnicode);
            s->getFont()->setFaceName(newFaceName);
        }

        //std::cout << percentageStr(endPos, totalCharsQty);
        s->setStartPos(endPos);
    } while ( endPos < totalCharsQty - 1 );

    string_t outputDir = Poco::Path(outputFolder_).makeAbsolute().makeDirectory().toString();
    doc->saveAs( outputDir + p.getBaseName() + "_Unicode." + p.getExtension() );
    writeFileAsBinary( outputDir + p.getBaseName() + ".txt", docAsText);

    std::stringstream ss;
    ss << "Fonts found in the document '" << fileName << "' are: \n";
    for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it) {
        ss << *it << std::endl;
    }
    
    //writeFileAsBinary( outputDir + "Fonts_" + p.getBaseName() + "_Unicode.txt", ss.str());
    logInfo(logger(), ss.str());
}

string_t Converter::percentageStr( int current, int total )
{
    std::stringstream ss;
    ss << 100 * (int)((double) current / (double) total) << " % completed.";
    return ss.str();
}




/*
void Converter::loadFonts()
{
    FileFinder ff(false, mappingFolder_, "ttf");
    auto files = ff.getFiles();

    HDC hdc = CreateCompatibleDC(NULL);

    for (auto it = files.begin(); it != files.end(); ++it) {
        string_t rawData;
        readFileAsBinary(ff.getRootPath() + *it, rawData);

        DWORD dwNumFonts = 0;
        void* fontData = (void*)&rawData[0];
        DWORD fontSize = rawData.size();
        HANDLE fontHandle = ::AddFontMemResourceEx(fontData, fontSize, NULL, &dwNumFonts);

        /*
        ULONG ulStatus;
        LONG retVal = TTLoadEmbeddedFont(fontHandle, 0, EMBED_PREVIEWPRINT, LICENSE_DEFAULT, &ulStatus, );

        //HANDLE font = CreateFont();
        //SelectFont()
        //GetTextFace
        
        // GCP_DBCS      
        // GCP_DIACRITIC 
        // FLI_GLYPHS    
        // GCP_GLYPHSHAPE
        // GCP_KASHIDA   
        // GCP_LIGATE    
        // GCP_USEKERNING
        // GCP_REORDER   
        GetFontLanguageInfo(hdc);
    }

    DeleteDC(hdc);
}
*/