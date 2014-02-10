#include "StdAfx.h"
#include "WordApp.h"
#include "../utils/Common.h"
#include "../automation/OLEMethod.h"

/// ----------------------------------------------------------------------------
WordApp::WordApp()
    : wordApp_(0)
{
    initialize(false);
}

/// ----------------------------------------------------------------------------
WordApp::~WordApp()
{
    quit();
}

/// ----------------------------------------------------------------------------
HRESULT WordApp::initialize( bool visible )
{
    CLSID clsid;
    hr_ = CLSIDFromProgID(L"Word.Application", &clsid);
    Validation2("CLSIDFromProgID", hr_);

    hr_ = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, 
        IID_IDispatch, (void **)& wordApp_);
    Validation2("CoCreateInstance", hr_);

    setVisible(false);
    return hr_;
}

/// ----------------------------------------------------------------------------
void WordApp::setVisible( bool visible )
{
    VARIANT x;
    x.vt = VT_I4;
    x.lVal = visible;
    OLEMethod(DISPATCH_PROPERTYPUT, NULL, wordApp_, L"Visible", 1, x);
}

/// ----------------------------------------------------------------------------
void WordApp::quit()
{
    if (!wordApp_)
        return;

    docs_->closeAll();
    docs_.reset();

    hr_ = OLEMethod(DISPATCH_METHOD, NULL, wordApp_, L"Quit", 0);
    SafeRelease(wordApp_);
}

tDocumentsSp WordApp::getDocuments()
{
    if (!docs_) {
        VARIANT result;
        VariantInit(&result);
        hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, wordApp_, L"Documents", 0);
        docs_.reset(new Documents(result.pdispVal));
    }
    return docs_;
}

tSelectionSp WordApp::getSelection()
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, wordApp_, L"Selection", 0);
    return tSelectionSp(new Selection(result.pdispVal));
}

/*
tDocumentSp WordApp::getActiveDocument()
{
    if (wordApp_) {
        VARIANT result;
        VariantInit(&result);
        hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, wordApp_, L"Application", 0);
        IDispatch* app = result.pdispVal;
        tDocumentSp doc(new Document(app));
        return doc;
    }
    return tDocumentSp();    
}
*/