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

string_t Font::getFaceName() const
{
    return toUtf8(getPropStr(font_, L"Name"));
}

void Font::setFaceName( const string_t& faceName )
{
    setPropStr(font_, L"Name", toUtf16(faceName));
}

