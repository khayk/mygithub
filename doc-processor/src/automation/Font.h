#ifndef WORD_FONT_H
#define WORD_FONT_H

#include "Typedefs.h"

class Font
{
public:
    Font(IDispatch* font);
    ~Font();
    
    void reset();

    void setNameAscii(const string_t& faceName);
    string_t getNameAscii() const;

    void setName(const string_t& faceName);
    string_t getName() const;
private:
    IDispatch* font_;
};

typedef boost::shared_ptr<Font> tFontSp;

#endif