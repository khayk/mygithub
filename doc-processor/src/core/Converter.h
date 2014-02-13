#ifndef ASCII_TO_UNICODE_CONVERTER_H
#define ASCII_TO_UNICODE_CONVERTER_H

#include "../automation/WordApp.h"
#include "../RootLogger.h"

#include <map>

class CharMapping : public LogSource {
public:
    CharMapping(const string_t& mapFile);
    ~CharMapping();

    void doConversion(const wstring_t& asciiText, wstring_t& unicodeText);

private:
    void initDefaultMappings();
    wchar_t lookUp(wchar_t ch);

    std::map<wchar_t, wchar_t> mapping_;
    string_t replacingFont_;

    static const int QUICK_MAP = 256;
    wchar_t quickMap_[QUICK_MAP];
};

typedef boost::shared_ptr<CharMapping> tCharMappingSp;

/// 
class Converter : public LogSource {
public:
    Converter();
    ~Converter();

    void initialize(
        const string_t& inputFolder, 
        const string_t& outputFolder,
        const string_t& mappingFolder);
    void start();

private:
    void convertSingleDoc(const string_t& fileName);
    void loadFonts();

    std::map<string_t, tCharMappingSp> fontMaps_;
    tWordAppSp word_;

    string_t inputFolder_;
    string_t outputFolder_;
    string_t mappingFolder_;
    
    std::map<string_t, string_t> fontNameMap_;
};

#endif ASCII_TO_UNICODE_CONVERTER_H
