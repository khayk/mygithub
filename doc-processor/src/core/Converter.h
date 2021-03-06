#ifndef ASCII_TO_UNICODE_CONVERTER_H
#define ASCII_TO_UNICODE_CONVERTER_H

#include "../automation/WordApp.h"
#include "../excel/ExcelApp.h"
#include "../RootLogger.h"
#include "Security.h"

#include <Poco/Util/AbstractConfiguration.h>
typedef Poco::AutoPtr<Poco::Util::AbstractConfiguration>    tConfigPtr;

class CharMapping : public LogSource {
public:
    CharMapping();
    CharMapping(const string_t& mapFile);
    ~CharMapping();

    /// returns true if there are only spacing symbols in the text
    /// font name used only for logging purposes if there occurred an error
    /// during conversion
    bool doConversion(const wstring_t& asciiText, wstring_t& unicodeText,
        const string_t& fontName);

    /// update the mapping of the characters
    void updateCharMapping(const string_t& mapFile);

    string_t getLanguage() const;
    void setLanguage(const string_t& language);

private:
    void initDefaultMappings();
    void loadMappingFile(const string_t& mapFile);

    wchar_t lookUp(wchar_t ch, const string_t& fontName);

    std::map<wchar_t, wchar_t> mapping_;
    string_t language_;

    static const int QUICK_MAP = 256;
    wchar_t quickMap_[QUICK_MAP];
};

typedef std::shared_ptr<CharMapping> tCharMappingSp;

///
class Converter : public LogSource {
public:
    Converter(const tConfigPtr& config);
    ~Converter();

    void start();

private:
	tWordAppSp& word();
	tExcelAppSp& excel();

    void initialize(
        const string_t& inputFolder,
        const string_t& outputFolder,
        const string_t& mappingFolder);

	void convertSingleExcel(const std::string& fileName);

    void convertSingleDocQuick(const string_t& fileName);
    void convertSingleDocPrecise(const string_t& fileName);

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

    tConfigPtr  config_;
    tWordAppSp  word_;
	tExcelAppSp excel_;

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

    string_t getInputAbsPath(const string_t& name);
    string_t getOutputAbsPath(const string_t& name);
    void     logUsedFonts(const string_t& name, std::set<string_t>& usedFonts);

    wstring_t processRangePrecise(tRangeSp& r, bool showProgress); /// This is one solution
    wstring_t processRangePreciseVer2(tRangeSp& r, bool showProgress);

	void convertText(tExcelRangeSp& r, wstring_t& text, wstring_t& textUnicode);

    void processRangeClassic(tRangeSp& r, wstring_t& text, wstring_t& textUnicode);
    void processRangeClassic2(tRangeSp& r, wstring_t& text, wstring_t& textUnicode);


    void processRangeHelper(tRangeSp& r, wstring_t& text, wstring_t& textUnicode, int pos);

    /// used font list
    std::set<string_t> usedFonts_;

    wstring_t       specialChars_;
    wstring_t       specialCharsArafi_;

    /// used to return reference of empty mapping
    tCharMappingSp  noMapping;
    int             savedStart_;
    int             savedEnd_;
    bool            hasSavedSelection_;

    bool            wordVisible_;
	bool            excelVisible_;
    bool            wantUtf8Text_;

#ifdef SECURITY_ENABLED
    Security        security_;
#endif
};

#endif ASCII_TO_UNICODE_CONVERTER_H
