#ifndef ASCII_TO_UNICODE_CONVERTER_H
#define ASCII_TO_UNICODE_CONVERTER_H

#include "../automation/WordApp.h"
#include "../RootLogger.h"

#include <Poco/Util/AbstractConfiguration.h>
#include <map>

typedef Poco::AutoPtr<Poco::Util::AbstractConfiguration>    tConfigPtr;

class CharMapping : public LogSource {
public:
    CharMapping();
    CharMapping(const string_t& mapFile);
    ~CharMapping();

    /// returns true if there are only spacing symbols in the text
    bool doConversion(const wstring_t& asciiText, wstring_t& unicodeText);

    /// update the mapping of the characters
    void updateCharMapping(const string_t& mapFile); 

    string_t getLanguage() const;
    void setLanguage(const string_t& language);

private:
    void initDefaultMappings();
    void loadMappingFile(const string_t& mapFile);

    wchar_t lookUp(wchar_t ch);

    std::map<wchar_t, wchar_t> mapping_;
    string_t language_;

    static const int QUICK_MAP = 256;
    wchar_t quickMap_[QUICK_MAP];
};

typedef boost::shared_ptr<CharMapping> tCharMappingSp;

/// 
class Converter : public LogSource {
public:
    Converter(const tConfigPtr& config);
    ~Converter();

    void start();

    /// in quick mode there will be lost information about text decoration, such
    /// color, style, bold, underline. But there will be kept information about
    /// font size.
    /// slow mode work very slowly and keep all formating on the document while
    /// performing conversion
    void setMode(bool quick);

private:
    void initialize(
        const string_t& inputFolder, 
        const string_t& outputFolder,
        const string_t& mappingFolder);

    void convertSingleDoc(const string_t& fileName);
    
    void loadKnownAsciiFonts(const string_t& languageDir, const string_t& language);
    void loadNamesMapping(const string_t& fileName, 
                          const string_t& language, 
                          bool optional = false);
    void loadFontList(const string_t& fileName, std::set<string_t>& cnt);
    string_t percentageStr(int current, int total);

    std::map<string_t, tCharMappingSp> fontCharMaps_;
    std::map<string_t, string_t>       fontNameMap_;

    std::set<string_t> fontsAllCharSupport_;
    std::set<string_t> fontsToIgnore_;

    /// if font does not found in the fonts name list this one
    /// will be used for substitution
    std::map<string_t, string_t> defaultFonts_;

    tConfigPtr config_;
    tWordAppSp word_;
    bool       quickMode_;

    string_t   inputFolder_;
    string_t   outputFolder_;
    string_t   mappingFolder_;

private:
    /// some helping stuff
    bool isUnicodeFont(const string_t& name) const;
    bool isIgnoredFont(const string_t& name) const;
    bool canSkipFont(const string_t& name) const;
    tCharMappingSp& getCM(const string_t& font);
    void saveSelection(tSelectionSp& s);
    void restoreSelection(tSelectionSp& s);
    void resetSavedSelection();
    string_t makeGuess(tSelectionSp& s);
    string_t getFontSubstitution(const tCharMappingSp& cm, const string_t& fontName);

    /// used to return reference of empty mapping
    tCharMappingSp noMapping;
    int    savedStart_;
    int    savedEnd_;
    bool   hasSavedSelection_;
};

#endif ASCII_TO_UNICODE_CONVERTER_H
