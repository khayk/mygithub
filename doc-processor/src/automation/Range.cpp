#include "StdAfx.h"
#include "Range.h"
#include "OLEMethod.h"
#include "Selection.h"

Range::Range( IDispatch* range )
    : Selection(range) //range_(range)
{

}

Range::~Range()
{

}

// wstring_t Range::getText() const
// {
//     return getPropStr(s_, L"Text");
// }
// 
// void Range::setText( const wstring_t& text )
// {
//     setPropStr(s_, L"Text", text);
// }
