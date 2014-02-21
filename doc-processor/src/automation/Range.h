#pragma once

#include "BaseRS.h"

class Range : public BaseRS
{
public:
    Range(IDispatch* range);

    tRangeSp getNextStoryRange();
    tRangeSp getNext();
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


class Paragraph;
typedef boost::shared_ptr<Paragraph>  tParagraphSp;

class Paragraph {
public:
    Paragraph(IDispatch* paragraph);
    ~Paragraph();

    IDispatch*   getIDispatch() const;
    tParagraphSp getNext();
    tRangeSp     getRange();

private:
    IDispatch* paragraph_;
};


/// ----------------------------------------------------------------------------
class Paragraphs {
public:
    Paragraphs(IDispatch* paragraphs);
    ~Paragraphs();

    int getCount();
    tParagraphSp getItem(int index);
    tParagraphSp getFirst();

private:
    IDispatch* paragraphs_;
};


typedef boost::shared_ptr<Range>       tRangeSp;
typedef boost::shared_ptr<Characters>  tCharactersSp;
typedef boost::shared_ptr<Paragraphs>  tParagraphsSp;
typedef boost::shared_ptr<StoryRanges> tStoryRangesSp;