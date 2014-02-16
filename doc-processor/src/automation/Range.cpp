#include "StdAfx.h"
#include "Range.h"
#include "OLEMethod.h"

Range::Range( IDispatch* range )
    : range_(range)
{

}

Range::~Range()
{

}

wstring_t Range::getText() const
{
    return getPropStr(range_, L"Text");
}

void Range::setText( const wstring_t& text )
{
    setPropStr(range_, L"Text", text);
}
