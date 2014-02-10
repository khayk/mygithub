#include "StdAfx.h"
#include "Document.h"
#include "OLEMethod.h"

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
