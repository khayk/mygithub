#pragma once

#include "BaseRS.h"

class Range;
typedef boost::shared_ptr<Range> tRangeSp;

#define BEGIN_OBJECT_FROM_IDISP(NEW_CLASS, BASE_CLASS)   \
class NEW_CLASS : BASE_CLASS{                            \
public:                                                  \
    NEW_CLASS(IDispatch* disp) : BASE_CLASS(disp) {}     \

#define END_OBJECT(NEW_CLASS)                            \
};                                                       \
    typedef boost::shared_ptr<NEW_CLASS> t##NEW_CLASS##Sp;

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Note, BaseObject)
END_OBJECT(Note)

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Notes, BaseObject)
END_OBJECT(Notes)

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Footnotes, Notes)
END_OBJECT(Footnotes)

class Range : public BaseRS
{
public:
    Range(IDispatch* range);

    void autoFormat();
    int  getHighlightColorIndex();

    tFootnotesSp getFootnotes();
    
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

    IDispatch*    getIDispatch() const;
    tParagraphSp  getNext();
    tRangeSp      getRange();

    tBaseObjectSp getStyle();
    void setStyle(const tBaseObjectSp& obj);
    tBaseObjectSp getFormat();
    void setFormat(const tBaseObjectSp& obj);

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