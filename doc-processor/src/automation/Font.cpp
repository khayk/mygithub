#include "StdAfx.h"
#include "Font.h"
#include "OLEMethod.h"
#include "../utils/Common.h"

Font::Font( IDispatch* font )
    : font_(font)
{

}

Font::~Font()
{
    SafeRelease(font_);
}

string_t Font::getName() const
{
    return toUtf8(getPropStr(font_, L"Name"));
}

void Font::setName( const string_t& faceName )
{
    setPropStr(font_, L"Name", toUtf16(faceName));
}

void Font::reset()
{
    OLEMethod(DISPATCH_METHOD, NULL, font_, L"Reset", 0);
}

void Font::setSize( int sz )
{
    setPropertyInt(font_, L"Size", sz);
}

void Font::setColor( COLORREF clr )
{
    setPropertyInt(font_, L"Color", clr);
}

void Font::setBold( int bold )
{
    setPropertyInt(font_, L"Bold", bold);
}

void Font::setUnderlineColor( COLORREF clr )
{
    setPropertyInt(font_, L"UnderlineColor", clr);
}

void Font::setUnderline( int underline )
{
    setPropertyInt(font_, L"Underline", underline);
}

void Font::setItalic( bool italic )
{
    setPropertyInt(font_, L"Italic", (int)italic);
}

int Font::getSize() const
{
    return (int) getPropertyFloat(font_, L"Size");
}

COLORREF Font::getColor() const
{
    return getPropertyInt(font_, L"Color");
}

int Font::getBold() const
{
    return getPropertyInt(font_, L"Bold");
}

COLORREF Font::getUnderlineColor() const
{
    return getPropertyInt(font_, L"UnderlineColor");
}

int Font::getUnderline() const
{
    return getPropertyInt(font_, L"Underline");
}

bool Font::getItalic() const
{
    return (getPropertyInt(font_, L"Italic") ? true : false);
}

IDispatch* Font::getIDispatch()
{
    return font_;
}

tFontSp Font::duplicate()
{
    return tFontSp(new Font(getPropertyDispatch(font_, L"Duplicate")) );
}

bool Font::haveCommonAttributes()
{
    string_t nn = getName();
    int     sz  = getSize();
    COLORREF c  = getColor();
    int rval = GetRValue(c), gVal = GetGValue(c), bVal = GetBValue(c);

    int      b  = getBold();
    COLORREF uc = getUnderlineColor();
    int      u  = getUnderline();
    bool     i  = getItalic();
    return true;
}

