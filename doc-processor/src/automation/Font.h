#ifndef WORD_FONT_H
#define WORD_FONT_H

#include "Typedefs.h"

class Font
{
public:
    Font(IDispatch* font);
    ~Font();
    
    string_t getFaceName() const;
private:
    IDispatch* font_;
};

typedef boost::shared_ptr<Font> tFontSp;

#endif