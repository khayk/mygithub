#include "StdAfx.h"
#include "MSWord.h"
#include "../utils/Common.h"
#include "../automation/OLEMethod.h"

#define SafeRelease(p)      if (p) { p->Release(); p = 0; }
#define Validation3(fnName, param, hr)           \
{                                                \
    if (FAILED(hr)) {                            \
        reportFailure(fnName, param, hr);        \
        return E_FAIL;                           \
    }                                            \
}

#define Validation2(fnName, hr)    Validation3(fnName, string_t(""), hr)

MSWord::MSWord()
{
    wordApp_   = 0;
    activeDoc_ = 0;
    docs_      = 0;
    app_       = 0;
    hr_        = S_OK;
    initialize(false);
}

MSWord::~MSWord()
{
    quit();
}

HRESULT MSWord::initialize( bool visible /*= true*/ )
{
    CLSID clsid;
    hr_ = CLSIDFromProgID(L"Word.Application", &clsid);
    Validation2("CLSIDFromProgID", hr_);

    hr_ = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, 
        IID_IDispatch, (void **)&wordApp_);
    Validation2("CoCreateInstance", hr_);

    setVisible(visible);
    return hr_;
}

void MSWord::setVisible( bool visible /*= true*/ )
{
    VARIANT x;
    x.vt = VT_I4;
    x.lVal = visible;
    hr_ = OLEMethod(DISPATCH_PROPERTYPUT, NULL, wordApp_, L"Visible", 1, x);
}

void MSWord::open( const wstring_t& file, bool visible )
{
    if (!wordApp_) {
        initialize(visible);
    }

    { // GetDocuments
        VARIANT result;
        VariantInit(&result);
        hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, wordApp_, L"Documents", 0);
        SafeRelease(docs_);
        docs_ = result.pdispVal;
    }
    
    { // OpenDocument
        VARIANT result;
        VariantInit(&result);
        VARIANT fname;
        fname.vt = VT_BSTR;
        fname.bstrVal = ::SysAllocString(file.c_str());

        hr_ = OLEMethod(DISPATCH_METHOD, &result, docs_, L"Open", 1, fname);
        SafeRelease(activeDoc_);
        activeDoc_ = result.pdispVal;

        SysFreeString(fname.bstrVal);
    }

    {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, activeDoc_, L"Application", 0);
        SafeRelease(app_);
        app_ = result.pdispVal;
    }

    // TODO: maybe remove this
    setVisible(visible);
}

void MSWord::closeActiveDocument( bool save /*= false*/ )
{
    if (!wordApp_ || !activeDoc_)
        return;

    VARIANT result;
    VariantInit(&result);
    COleVariant varSave;
    varSave.boolVal = save;
    hr_ = OLEMethod(DISPATCH_METHOD, NULL, activeDoc_, L"Close", 1, varSave.Detach());
    SafeRelease(activeDoc_);
}

void MSWord::closeAll(bool save)
{
    if (!wordApp_ || !docs_)
        return;

    SafeRelease(activeDoc_);

    COleVariant varSave;
    varSave.boolVal = save;
    hr_ = OLEMethod(DISPATCH_METHOD, NULL, docs_, L"Close", 1, varSave.Detach());
    SafeRelease(docs_);
}


void MSWord::quit()
{
    if (!wordApp_)
        return;

    closeAll(false);

    hr_ = OLEMethod(DISPATCH_METHOD, NULL, wordApp_, L"Quit", 0);
    if (wordApp_) {
        wordApp_->Release();
        wordApp_ = 0;
    }
}

void MSWord::setFont( const wstring_t& faceName )
{

}
