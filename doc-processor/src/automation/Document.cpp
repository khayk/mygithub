#include "StdAfx.h"
#include "Document.h"
#include "OLEMethod.h"
#include "../utils/Common.h"

Document::Document(IDispatch* doc)
    : doc_(doc)
{
}

Document::~Document()
{
    SafeRelease(doc_);
}

void Document::close()
{
    if (doc_) {
        OLEMethod(DISPATCH_METHOD, NULL, doc_, L"Close", 0);
        SafeRelease(doc_);
    }
}

void Document::save()
{
    if (doc_) 
        OLEMethod(DISPATCH_METHOD, NULL, doc_, L"Save", 0);
}

void Document::saveAs( const string_t& fullPath )
{
    if (doc_) {
        VARIANT fname;
        VariantInit(&fname);
        fname.vt = VT_BSTR;
        fname.bstrVal = ::SysAllocString(toUtf16(fullPath).c_str());

        OLEMethod(DISPATCH_METHOD, NULL, doc_, L"SaveAs", 1, fname);
        SysFreeString(fname.bstrVal);
    }
}


tRangeSp Document::getContent()
{
    return tRangeSp(new Range(getPropertyDispatch(doc_, L"Content")) );
}

tStoryRangesSp Document::getStoryRanges()
{
    return tStoryRangesSp(new StoryRanges(getPropertyDispatch(doc_, L"StoryRanges")) );
}

tCharactersSp Document::getCharacters()
{
    return tCharactersSp(new Characters(getPropertyDispatch(doc_, L"Characters")) );
}

tParagraphsSp Document::getParagraphs()
{
    return tParagraphsSp(new Paragraphs(getPropertyDispatch(doc_, L"Paragraphs")) );
}

tFootnotesSp Document::getFootnotes()
{
    return tFootnotesSp(new Footnotes(getPropertyDispatch(doc_, L"Footnotes")) );
}

tStylesSp Document::getStyles()
{
    return tStylesSp(new Styles(getPropertyDispatch(doc_, L"Styles")) );
}

tFormFieldsSp Document::getFormFields()
{
    return tFormFieldsSp(new FormFields(getPropertyDispatch(doc_, L"FormFields")) );
}

tSectionsSp Document::getSections()
{
    return tSectionsSp(new Sections(getPropertyDispatch(doc_, L"Sections")) );
}

tSentencesSp Document::getSentences()
{
    return tSentencesSp(new Sentences(getPropertyDispatch(doc_, L"Sentences")) );
}
