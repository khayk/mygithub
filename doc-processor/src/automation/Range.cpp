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
