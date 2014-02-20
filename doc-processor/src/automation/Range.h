#pragma once

#include "BaseRS.h"

class Range : public BaseRS
{
public:
    Range(IDispatch* range);

    tRangeSp getNextStoryRange();
};


class StoryRanges {
public:
    StoryRanges(IDispatch* range);
    ~StoryRanges();

    int getCount();
protected:
    IDispatch* range_;
};

typedef boost::shared_ptr<Range> tRangeSp;
typedef boost::shared_ptr<StoryRanges> tStoryRangesSp;