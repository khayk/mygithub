#pragma once

#include "BaseRS.h"
#include "Find.h"

class Range;
typedef boost::shared_ptr<Range> tRangeSp;

#define BEGIN_OBJECT_FROM_IDISP(NEW_CLASS, BASE_CLASS)   \
class NEW_CLASS : public BASE_CLASS{                     \
public:                                                  \
    NEW_CLASS(IDispatch* disp) : BASE_CLASS(disp) {}     \

#define END_OBJECT(NEW_CLASS)                            \
};                                                       \
    typedef boost::shared_ptr<NEW_CLASS> t##NEW_CLASS##Sp;

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Note, BaseObject)
    tRangeSp getRange() const;
END_OBJECT(Note)

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Notes, BaseObject)
public:
    int getCount() const;
    tNoteSp getItem(int index);
END_OBJECT(Notes)


/*
///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Endnote, Note)
END_OBJECT(Endnote)

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Endnotes, Notes)
END_OBJECT(Endnotes)

*/

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Footnote, Note)
END_OBJECT(Footnote)

///-----------------------------------------------------------------------------
BEGIN_OBJECT_FROM_IDISP(Footnotes, Notes)
END_OBJECT(Footnotes)

///-----------------------------------------------------------------------------
#define ADD_NOTES_INTERFACE(idisp)     \
    tFootnotesSp getFootnotes() {      \
    return tFootnotesSp(new Footnotes(getPropertyDispatch(idisp, L"Footnotes")) ); \
}    


class Range : public BaseRS
{
public:
    Range(IDispatch* range);

    void autoFormat();
    int  getHighlightColorIndex();

    tFootnotesSp getFootnotes();
    
    tRangeSp getNextStoryRange();
    tRangeSp getNext();

    tFindSp getFind();

    void collapse();
};


/// ----------------------------------------------------------------------------
class StoryRanges {
public:
    StoryRanges(IDispatch* range);
    ~StoryRanges();

    int getCount();
protected:
    IDispatch* range_;
};


/// ----------------------------------------------------------------------------
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


/// ----------------------------------------------------------------------------
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


/// ----------------------------------------------------------------------------
class Style {
public:
    Style(IDispatch* style);
    ~Style();

    //const VARIANT& get() const;
private:
    IDispatch* style_;
};

typedef boost::shared_ptr<Style>      tStyleSp;

/// ----------------------------------------------------------------------------
class Styles {
public:
    Styles(IDispatch* styles) : styles_(styles) {}
    ~Styles() { SafeRelease(styles_); }

    int      getCount();
    tStyleSp getItem(int index);

private:
    IDispatch* styles_;
};

typedef boost::shared_ptr<Styles>      tStylesSp;
typedef boost::shared_ptr<Range>       tRangeSp;
typedef boost::shared_ptr<Characters>  tCharactersSp;
typedef boost::shared_ptr<Paragraphs>  tParagraphsSp;
typedef boost::shared_ptr<StoryRanges> tStoryRangesSp;