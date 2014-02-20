#ifndef WORD_FONT_H
#define WORD_FONT_H

#include "Typedefs.h"

class Font;
typedef boost::shared_ptr<Font> tFontSp;

class Font
{
public:
    Font(IDispatch* font);
    ~Font();
    
    void       reset();
    tFontSp    duplicate();
    IDispatch* getIDispatch();
    bool       haveCommonAttributes();

    void       setName(const string_t& faceName);
    void       setSize(int sz);
    void       setColor(COLORREF clr);

    /// -1 to set bold, 0 to remove bold, 1 to toggle
    void       setBold(int bold);
    void       setUnderlineColor(COLORREF clr);
    void       setUnderline(int underline);
    void       setItalic(bool italic);

    string_t   getName() const;
    int        getSize() const;
    COLORREF   getColor() const;
    int        getBold() const;
    COLORREF   getUnderlineColor() const;
    int        getUnderline() const;
    bool       getItalic() const;

private:
    IDispatch* font_;
};

#endif