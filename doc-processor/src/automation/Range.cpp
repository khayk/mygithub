#include "StdAfx.h"
#include "Range.h"
#include "OLEMethod.h"
#include "Selection.h"

Range::Range( IDispatch* range )
    : BaseRS(range)
{
}

tRangeSp Range::getNextStoryRange()
{
    return tRangeSp(new Range(getPropertyDispatch(base_, L"NextStoryRange")) );
}


StoryRanges::StoryRanges( IDispatch* range )
    : range_(range)
{

}

StoryRanges::~StoryRanges()
{
    SafeRelease(range_);
}

int StoryRanges::getCount()
{
    return getPropertyInt(range_, L"Count");
}


Characters::~Characters()
{
    SafeRelease(chars_);
}

int Characters::getCount()
{
    return getPropertyInt(chars_, L"Count");
}

tRangeSp Characters::getItem( int index )
{
    VARIANT result;
    VARIANT input;
    VariantInit(&result);
    VariantInit(&input);

    input.vt = VT_I4;
    input.intVal = index;
    OLEMethod(DISPATCH_PROPERTYGET, &result, chars_, L"Item", 1, input);
    
    return tRangeSp(new Range(result.pdispVal));
}

tRangeSp Characters::getFirst()
{
    return tRangeSp(new Range(getPropertyDispatch(chars_, L"First")) );
}
