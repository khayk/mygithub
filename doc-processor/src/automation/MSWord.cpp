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


/// ----------------------------------------------------------------------------
MSWord::MSWord()
{
    wordApp_   = 0;
    activeDoc_ = 0;
    docs_      = 0;
    app_       = 0;
    hr_        = S_OK;
    initialize(false);
}

/// ----------------------------------------------------------------------------
MSWord::~MSWord()
{
    quit();
}

/// ----------------------------------------------------------------------------
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

/// ----------------------------------------------------------------------------
void MSWord::setVisible( bool visible /*= true*/ )
{
    VARIANT x;
    x.vt = VT_I4;
    x.lVal = visible;
    hr_ = OLEMethod(DISPATCH_PROPERTYPUT, NULL, wordApp_, L"Visible", 1, x);
}

/// ----------------------------------------------------------------------------
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
        selection_.reset();
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

/// ----------------------------------------------------------------------------
void MSWord::closeActiveDocument( bool save /*= false*/ )
{
    if (!wordApp_ || !activeDoc_)
        return;

    VARIANT result;
    VariantInit(&result);
    VARIANT varSave;
    varSave.vt = VT_BOOL;
    varSave.boolVal = save;
    hr_ = OLEMethod(DISPATCH_METHOD, NULL, activeDoc_, L"Close", 1, varSave);
    SafeRelease(activeDoc_);
}

/// ----------------------------------------------------------------------------
void MSWord::closeAll(bool save)
{
    if (!wordApp_ || !docs_)
        return;

    VARIANT varSave;
    varSave.vt = VT_BOOL;
    varSave.boolVal = save;

    hr_ = OLEMethod(DISPATCH_METHOD, NULL, docs_, L"Close", 1, varSave);
    SafeRelease(activeDoc_);
    SafeRelease(docs_);
}

/// ----------------------------------------------------------------------------
void MSWord::quit()
{
    if (!wordApp_)
        return;

    closeAll(false);

    hr_ = OLEMethod(DISPATCH_METHOD, NULL, wordApp_, L"Quit", 0);
    SafeRelease(wordApp_);
}

/// ----------------------------------------------------------------------------
void MSWord::setFont( const wstring_t& faceName )
{

}

/// ----------------------------------------------------------------------------
void MSWord::moveCursor( MoveDirection md, bool selectWhileMoving /*= false*/ )
{
    tDispatchIp selection = getSelection();

	VARIANT wdCharacter, wdExtend,Count;
	wdCharacter.vt   = VT_I4;
	wdCharacter.lVal = 1;
	wdExtend.vt      = VT_I4;
	wdExtend.lVal    = selectWhileMoving ? 1 : 0;
	Count.vt         = VT_I4;
	Count.lVal       = 1;
	switch (md) {
	case mdLeft:
		OLEMethod(DISPATCH_METHOD, NULL, selection.get(), L"MoveLeft", 3, wdExtend, Count, wdCharacter);
		break;
	case mdRight:
		OLEMethod(DISPATCH_METHOD, NULL, selection.get(), L"MoveRight", 3, wdExtend, Count, wdCharacter);
		break;
	case mdUp:
		wdCharacter.lVal = 5;
		OLEMethod(DISPATCH_METHOD, NULL, selection.get(), L"MoveUp", 3, wdExtend, Count, wdCharacter);
		break;
	case mdDown:
		wdCharacter.lVal = 5;
		OLEMethod(DISPATCH_METHOD, NULL, selection.get(), L"MoveDown", 3, wdExtend, Count, wdCharacter);
		break;
	}
}

/// ----------------------------------------------------------------------------
wstring_t MSWord::getString( int length )
{
    return wstring_t();
}

/// ----------------------------------------------------------------------------
wstring_t MSWord::getSelectedString()
{
    tDispatchIp selection = getSelection();
    wstring_t text;
	{
        VARIANT result;
        VariantInit(&result);
        hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, selection.get(), L"Text", 0);
        text = result.bstrVal;
	}
    return text;
}

/// ----------------------------------------------------------------------------
void MSWord::selectAll()
{
    tDispatchIp selection = getSelection();
    hr_ = OLEMethod(DISPATCH_METHOD, NULL, selection.get(), L"WholeStory", 0);
}

MSWord::tDispatchIp MSWord::getSelection()
{
    if (!wordApp_ || !activeDoc_)
        return selection_;

    if (selection_)
        return selection_;

    if (app_ == 0) {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, activeDoc_, L"Application", 0);
        SafeRelease(app_);
        app_ = result.pdispVal;
    }

    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, app_, L"Selection", 0);
    selection_.reset(result.pdispVal);

    return selection_;
}
