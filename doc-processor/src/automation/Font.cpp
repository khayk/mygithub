#include "StdAfx.h"
#include "Font.h"
#include "OLEMethod.h"

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
    return getPropStr(font_, L"Name");
}

