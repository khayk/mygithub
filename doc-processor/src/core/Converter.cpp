#include "StdAfx.h"
#include "Converter.h"
#include "../utils/FileFinder.h"
#include "../utils/Common.h"
#include "../automation/Selection.h"
#include "../automation/WordApp.h"

#include <Poco/String.h>
#include <Poco/Path.h>
#include <sstream>
#include <fstream>

CharMapping::CharMapping( const string_t& mapFile )
    : LogSource("charMapping")
{
    memset(quickMap_, 0, sizeof(quickMap_));
}

CharMapping::~CharMapping()
{
}

wchar_t CharMapping::lookUp( wchar_t ch )
{
    return '|';
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


/// ----------------------------------------------------------------------------
///                               CONVERTER
/// ----------------------------------------------------------------------------
Converter::Converter()
    : LogSource("converter")
{
    word_.reset(new WordApp());
    word_->setVisible(true);
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

    FileFinder ff(false, mappingFolder_);
    auto files = ff.getFiles();
    for (auto it = files.begin(); it != files.end(); ++it) {
        Poco::Path p(*it);
        fontMaps_.insert( 
            std::make_pair(p.getBaseName(), 
                tCharMappingSp(new CharMapping(ff.getRootPath() + *it))));
    }

    /// load from font name mapping file
    std::ifstream inFile(mappingFolder_ + "FontNameMapping.txt", std::ios::in);
    if (!inFile) {
        throw std::exception(("Failed to open font names mapping file: " 
        + mappingFolder_ + "FontNameMapping.txt").c_str());
    }

    while (inFile) {
        string_t singleLine;
        std::getline(inFile, singleLine);

        if (singleLine.empty())
            break;

        string_t::size_type pos = singleLine.find('|');
        string_t from = singleLine.substr(0, pos);
        string_t to   = singleLine.substr(pos + 1);

        from = Poco::trim(from);
        to   = Poco::trim(to);
        fontNameMap_.insert(std::make_pair(from, to));
    }
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

    do {
        s->selectCurrentFont();
        string_t fontName = s->getFont()->getFaceName();
        wstring_t text = s->getSelectionText(), textUnicode;

        startPos = s->getStartPos();
        endPos = s->getEndPos();

        bool restoreSelection = false;
        if (fontName.empty()) {
            /// try to do guess
            /// select the font of the middle symbol of the text
            int tmpStartPos = s->getStartPos();
            int tmpEndPos   = s->getEndPos();
            int midPos = (tmpStartPos + tmpEndPos) / 2;
            s->setStartPos(midPos);
            s->moveCursor(Selection::mdRight, false);
            fontName = s->getFont()->getFaceName();
            restoreSelection = true;
        }

        auto it = fontMaps_.find(fontName);
        if (it == fontMaps_.end()) {
            logError(logger(), "Font '" + fontName + "' is not found in mapping folder");
            /// create empty mapping to avoid repeated errors
            it = fontMaps_.insert(std::make_pair(fontName, tCharMappingSp())).first;
        }

        if (it->second) {
            it->second->doConversion(text, textUnicode);
            
            /// if we need to restore selection
            if (restoreSelection) {
                s->setStartPos(startPos);
                s->selectCurrentFont();
            }

            string_t newFaceName;
            auto fit = fontNameMap_.find(fontName);
            if (fit == fontNameMap_.end()) {
                logError(logger(), "Missing font name mapping for the font: " + fontName +
                    ". Add the mapping in the FontNameMapping.txt file.");
            }
            else {
                newFaceName = fit->second;
            }

            s->setSelectionText(textUnicode);
            s->getFont()->setFaceName(newFaceName);
        }

        s->setStartPos(endPos);

        if (fontName.empty()) {
            logError(logger(), "EMPTY FONT NAME: Investigate");
        }
        usedFonts.insert(fontName);
    } while ( endPos < totalCharsQty - 1 );

    string_t outputDir = Poco::Path(outputFolder_).makeAbsolute().makeDirectory().toString();
    doc->saveAs( outputDir + p.getFileName() );

    std::stringstream ss;
    ss << "Fonts found in the document '" << fileName << "' are: \n";
    for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it)
        ss << *it;
    
    writeFileAsBinary( outputDir + "fonts_" + p.getBaseName() + ".txt", ss.str());
    logInfo(logger(), ss.str());
}
