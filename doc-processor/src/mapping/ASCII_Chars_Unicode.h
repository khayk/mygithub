#ifndef ASCII_TO_UNICODE_MAPPING_H
#define ASCII_TO_UNICODE_MAPPING_H

class AsciiToUniMapping {
public:
    static void exportArmenianASCII_Unicode(const string_t& mapFile);
    static void exportRussianASCII_Unicode(const string_t& mapFile);

    static void createArafi_UnicodeMapping(const string_t& mapFile);
};

#endif