#ifndef WORD_FONT_H
#define WORD_FONT_H

#include "Typedefs.h"

class Font;
class Range;
typedef boost::shared_ptr<Font> tFontSp;
typedef boost::shared_ptr<Range> tRangeSp;

class Font
{
public:
    Font(IDispatch* font);
    ~Font();
    
    void       reset();
    tFontSp    duplicate();
    IDispatch* getIDispatch();
    bool       haveCommonAttributes(const tRangeSp& r, int& failedCheckIndex);

    void       setName(const string_t& faceName);
    void       setSize(int sz);
    void       setColor(COLORREF clr);

    /// -1 to set bold, 0 to remove bold, 1 to toggle
    void       setBold(int bold);
    void       setUnderlineColor(COLORREF clr);
    void       setUnderline(int underline);
    void       setItalic(int italic);

    string_t   getName() const;
    int        getSize() const;
    int        getColor() const;
    int        getBold() const;
    int        getUnderlineColor() const;
    int        getUnderline() const;
    int        getItalic() const;

private:
    IDispatch* font_;
};

#endif
