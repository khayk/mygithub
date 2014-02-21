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

tRangeSp Range::getNext()
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_METHOD, &result, base_, L"Next", 0);

    if (result.pdispVal)
        return tRangeSp(new Range(result.pdispVal) );
    return tRangeSp();
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


Characters::Characters( IDispatch* chars )
    : chars_(chars)
{

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


Paragraph::Paragraph( IDispatch* paragraph )
    : paragraph_(paragraph)
{

}

Paragraph::~Paragraph()
{
    SafeRelease(paragraph_);
}

tParagraphSp Paragraph::getNext()
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_METHOD, &result, paragraph_, L"Next", 0);

    if (result.pdispVal)
        return tParagraphSp(new Paragraph(result.pdispVal) );
    return tParagraphSp();
}

IDispatch* Paragraph::getIDispatch() const
{
    return paragraph_;
}

tRangeSp Paragraph::getRange()
{
    return tRangeSp(new Range(getPropertyDispatch(paragraph_, L"Range")) );
}

Paragraphs::Paragraphs( IDispatch* paragraph )
    : paragraphs_(paragraph)
{
}

Paragraphs::~Paragraphs()
{
    SafeRelease(paragraphs_);
}

int Paragraphs::getCount()
{
    return getPropertyInt(paragraphs_, L"Count");
}

tParagraphSp Paragraphs::getItem( int index )
{
    VARIANT result;
    VariantInit(&result);

    VARIANT x;
    //VariantInit(&input);
    x.vt = VT_BSTR;
    x.intVal = index;
    OLEMethod(DISPATCH_PROPERTYGET, &result, paragraphs_, L"Item", 1, x);

    return tParagraphSp(new Paragraph(result.pdispVal));
}

tParagraphSp Paragraphs::getFirst()
{
    return tParagraphSp(new Paragraph(getPropertyDispatch(paragraphs_, L"First")) );
}