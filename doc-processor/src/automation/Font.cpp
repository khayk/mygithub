#include "StdAfx.h"
#include "Font.h"
#include "OLEMethod.h"
#include "../utils/Common.h"
#include "Range.h"

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

void Font::setItalic( int italic )
{
    setPropertyInt(font_, L"Italic", italic);
}

int Font::getSize() const
{
    return (int) getPropertyFloat(font_, L"Size");
}

int Font::getColor() const
{
    return getPropertyInt(font_, L"Color");
}

int Font::getBold() const
{
    return getPropertyInt(font_, L"Bold");
}

int Font::getUnderlineColor() const
{
    return getPropertyInt(font_, L"UnderlineColor");
}

int Font::getUnderline() const
{
    return getPropertyInt(font_, L"Underline");
}

int Font::getItalic() const
{
    return getPropertyInt(font_, L"Italic");
}

IDispatch* Font::getIDispatch()
{
    return font_;
}

tFontSp Font::duplicate()
{
    return tFontSp(new Font(getPropertyDispatch(font_, L"Duplicate")) );
}

bool Font::haveCommonAttributes(const tRangeSp& r, int& fail)
{
    int i = 0;
    switch (fail) {
    case 0:  if (getSize()      == wdUndefined)               { fail = 0; return false; }
    case 1:  if (getBold()      == wdUndefined)               { fail = 1; return false; }
    case 2:  if (getItalic()    == wdUndefined)               { fail = 2; return false; }
    case 3:  if (getUnderline() == wdUndefined)               { fail = 3; return false; }
    case 4:  if (getColor()     == wdUndefined)               { fail = 4; return false; }
    case 5:  if (r->getHighlightColorIndex() == wdUndefined)  { fail = 5; return false; }
    case 6:  if (getName().empty())                           { fail = 6; return false; }
    }
    fail = 0;
    return true;
}

