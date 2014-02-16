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

void Font::setNameAscii( const string_t& faceName )
{
    setPropStr(font_, L"NameAscii", toUtf16(faceName));
}

string_t Font::getNameAscii() const
{
    return toUtf8(getPropStr(font_, L"NameAscii"));
}

void Font::reset()
{
    OLEMethod(DISPATCH_METHOD, NULL, font_, L"Reset", 0);
}

