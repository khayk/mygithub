#include "StdAfx.h"
#include "BaseRS.h"
#include "OLEMethod.h"
#include "Range.h"


BaseObject::BaseObject( IDispatch* disp )
    : disp_(disp)
{
}

BaseObject::~BaseObject()
{
    SafeRelease(disp_);
}

IDispatch* BaseObject::getIDispatch() const
{
    return disp_;
}


BaseRS::BaseRS( IDispatch* base )
    : BaseObject(base)
{
}

int BaseRS::getStoryLength() const
{
    return getPropertyInt(disp_, L"StoryLength");
}

tFontSp BaseRS::getFont() const
{
    return tFontSp(new Font(getPropertyDispatch(disp_, L"Font")) );
}

void BaseRS::setFont( const tFontSp& font )
{
    setPropertyDispatch(disp_, font->getIDispatch(), L"Font");
}

int BaseRS::setStart( int pos )
{
    return setPropertyInt(disp_, L"Start", pos);
}

int BaseRS::getStart() const
{
    return getPropertyInt(disp_, L"Start");
}

int BaseRS::setEnd( int pos )
{
    return setPropertyInt(disp_, L"End", pos);
}

int BaseRS::getEnd() const
{
    return getPropertyInt(disp_, L"End");
}

void BaseRS::setText( const wstring_t& text )
{
    setPropStr(disp_, L"Text", text);
}

wstring_t BaseRS::getText() const
{
    return getPropStr(disp_, L"Text");
}

void BaseRS::setFormattedText( const tRangeSp& range )
{
    setPropertyDispatch(disp_, range->getIDispatch(), L"FormattedText");
}

tRangeSp BaseRS::getFormattedText() const
{
    return tRangeSp(new Range(getPropertyDispatch(disp_, L"FormattedText")) );
}

void BaseRS::select()
{
    OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Select", 0);
}

void BaseRS::selectAll()
{
    OLEMethod(DISPATCH_METHOD, NULL, disp_, L"WholeStory", 0);
}
