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


class Characters {
public:
    Characters(IDispatch* chars);
    ~Characters();

    int getCount();
    tRangeSp getItem(int index);
    tRangeSp getFirst();

private:
    IDispatch* chars_;
};

typedef boost::shared_ptr<Range>       tRangeSp;
typedef boost::shared_ptr<Characters>  tCharactersSp;
typedef boost::shared_ptr<StoryRanges> tStoryRangesSp;