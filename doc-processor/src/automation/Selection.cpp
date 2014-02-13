#include "StdAfx.h"
#include "Selection.h"
#include "OLEMethod.h"

/// ----------------------------------------------------------------------------
Selection::Selection( IDispatch* selection )
    : s_(selection)
{

}

/// ----------------------------------------------------------------------------
Selection::~Selection()
{
    SafeRelease(s_);
}

/// ----------------------------------------------------------------------------
int Selection::moveCursor( MoveDirection md, bool selectWhileMoving )
{
    VARIANT result;
    VariantInit(&result);

    VARIANT wdCharacter, wdExtend,Count;
    wdCharacter.vt   = VT_I4;
    wdCharacter.lVal = 1;
    Count.vt         = VT_I4;
    Count.lVal       = 1;
    wdExtend.vt      = VT_I4;
    wdExtend.lVal    = selectWhileMoving ? 1 : 0;

    switch (md) {
    case mdLeft:
        OLEMethod(DISPATCH_METHOD, &result, s_, L"MoveLeft",  3, wdExtend, Count, wdCharacter);
        break;
    case mdRight:
        OLEMethod(DISPATCH_METHOD, &result, s_, L"MoveRight", 3, wdExtend, Count, wdCharacter);
        break;
    case mdUp:
        wdCharacter.lVal = 5;
        OLEMethod(DISPATCH_METHOD, &result, s_, L"MoveUp",    3, wdExtend, Count, wdCharacter);
        break;
    case mdDown:
        wdCharacter.lVal = 5;
        OLEMethod(DISPATCH_METHOD, &result, s_, L"MoveDown",  3, wdExtend, Count, wdCharacter);
        break;
    }

    return result.intVal;
}

/// ----------------------------------------------------------------------------
wstring_t Selection::getString( int length )
{
    return wstring_t();
}

/// ----------------------------------------------------------------------------
wstring_t Selection::getSelectionText()
{
    return getPropStr(s_, L"Text");
}

void Selection::setSelectionText( const wstring_t& text )
{
    setPropStr(s_, L"Text", text);
}

/// ----------------------------------------------------------------------------
void Selection::selectAll()
{
    OLEMethod(DISPATCH_METHOD, NULL, s_, L"WholeStory", 0);
}

/// ----------------------------------------------------------------------------
void Selection::selectCurrentFont()
{
    OLEMethod(DISPATCH_METHOD, NULL, s_, L"SelectCurrentFont", 0);
}

/// ----------------------------------------------------------------------------
int Selection::setStartPos( int newPos )
{
    return setPropertyInt(s_, L"Start", newPos);
}

/// ----------------------------------------------------------------------------
int Selection::setEndPos( int newPos )
{
    return setPropertyInt(s_, L"End", newPos);
}

/// ----------------------------------------------------------------------------
int Selection::getStartPos() const
{
    return getPropertyInt(s_, L"Start");
}

/// ----------------------------------------------------------------------------
int Selection::getEndPos() const
{
    return getPropertyInt(s_, L"End");
}

/// ----------------------------------------------------------------------------
int Selection::allCharactersCount() const
{
    return getPropertyInt(s_, L"StoryLength");
}

tFontSp Selection::getFont()
{
    return tFontSp(new Font(getPropertyDispatch(s_, L"Font")) );
}

