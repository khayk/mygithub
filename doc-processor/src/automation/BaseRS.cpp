#include "StdAfx.h"
#include "BaseRS.h"
#include "OLEMethod.h"
#include "Range.h"

BaseRS::BaseRS( IDispatch* base )
    : base_(base)
{
}

BaseRS::~BaseRS()
{
    SafeRelease(base_);
}

IDispatch* BaseRS::getIDispatch() const
{
    return base_;
}

int BaseRS::getStoryLength() const
{
    return getPropertyInt(base_, L"StoryLength");
}

tFontSp BaseRS::getFont() const
{
    return tFontSp(new Font(getPropertyDispatch(base_, L"Font")) );
}

void BaseRS::setFont( const tFontSp& font )
{
    setPropertyDispatch(base_, font->getIDispatch(), L"Font");
}

int BaseRS::setStart( int pos )
{
    return setPropertyInt(base_, L"Start", pos);
}

int BaseRS::getStart() const
{
    return getPropertyInt(base_, L"Start");
}

int BaseRS::setEnd( int pos )
{
    return setPropertyInt(base_, L"End", pos);
}

int BaseRS::getEnd() const
{
    return getPropertyInt(base_, L"End");
}

void BaseRS::setText( const wstring_t& text )
{
    setPropStr(base_, L"Text", text);
}

wstring_t BaseRS::getText() const
{
    return getPropStr(base_, L"Text");
}

void BaseRS::setFormattedText( const tRangeSp& range )
{
    setPropertyDispatch(base_, range->getIDispatch(), L"FormattedText");
}

tRangeSp BaseRS::getFormattedText() const
{
    return tRangeSp(new Range(getPropertyDispatch(base_, L"FormattedText")) );
}

void BaseRS::select()
{
    OLEMethod(DISPATCH_METHOD, NULL, base_, L"Select", 0);
}

void BaseRS::selectAll()
{
    OLEMethod(DISPATCH_METHOD, NULL, base_, L"WholeStory", 0);
}
