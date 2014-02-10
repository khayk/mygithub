#include "StdAfx.h"
#include "DocFile.h"
#include "../utils/FileFinder.h"
#include "../utils/Common.h"
#include "../automation/Selection.h"
#include "../automation/WordApp.h"

#include <Poco/Path.h>
#include <sstream>

/*
    // Get CLSID for Word.Application...
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
    if(FAILED(hr)) {
        ::MessageBox(NULL, "CLSIDFromProgID() failed", "Error", 
            0x10010);
        return -1;
    }

    // Start Word and get IDispatch...
    IDispatch *pWordApp;
    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, 
        IID_IDispatch, (void **)&pWordApp);
    if(FAILED(hr)) {
        ::MessageBox(NULL, "Word not registered properly", 
            "Error", 0x10010);
        return -2;
    }

    // Make Word visible
    {
        VARIANT x;
        x.vt = VT_I4;
        x.lVal = 1;
        OLEMethod(DISPATCH_PROPERTYPUT, NULL, pWordApp, L"Visible", 1, 
            x);
    }

    // Get Documents collection
    IDispatch *pDocs;
    {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, pWordApp, L"Documents", 
            0);

        pDocs = result.pdispVal;
    }

    // Call Documents.Open() to open file
    IDispatch *pDoc;
    {
        VARIANT result;
        VariantInit(&result);
        VARIANT x;
        x.vt = VT_BSTR;
        x.bstrVal = ::SysAllocString(wideName.c_str());

        OLEMethod(DISPATCH_METHOD, &result, pDocs, L"Open", 1, x);
        pDoc = result.pdispVal;
        SysFreeString(x.bstrVal);
    }

    // Get BuiltinDocumentProperties collection
    IDispatch *pProps;
    {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, pDoc, 
            L"BuiltinDocumentProperties", 0);
        pProps = result.pdispVal;
    }

    // Get "Subject" from BuiltInDocumentProperties.Item("Subject")
    IDispatch *pPropSubject;
    {
        VARIANT result;
        VariantInit(&result);
        VARIANT x;
        x.vt = VT_BSTR;
        x.bstrVal = ::SysAllocString(L"Subject");
        OLEMethod(DISPATCH_PROPERTYGET, &result, pProps, L"Item", 1, x);
        pPropSubject = result.pdispVal;
        SysFreeString(x.bstrVal);
    }

    // Get the Value of the Subject property and display it
    {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, pPropSubject, L"Value",
            0);
        char buf[512];
        size_t i;
        wcstombs_s(&i, buf, result.bstrVal, 512);
        ::MessageBox(NULL, buf, "Subject", 0x10000);

    }

    // Set the Value of the Subject DocumentProperty
    {
        VARIANT x;
        x.vt = VT_BSTR;
        x.bstrVal = ::SysAllocString(L"This is my subject");
        OLEMethod(DISPATCH_PROPERTYPUT, NULL, pPropSubject, L"Value", 1, 
            x);
        ::MessageBox(NULL, 
            "Subject property changed, examine document.",
            "Subject", 0x10000);
        SysFreeString(x.bstrVal);
    }

    // Get CustomDocumentProperties collection
    IDispatch *pCustomProps;
    {
        VARIANT result;
        VariantInit(&result);
        OLEMethod(DISPATCH_PROPERTYGET, &result, pDoc, 
            L"CustomDocumentProperties", 0);
        pCustomProps = result.pdispVal;
    }

    // Add a new property named "CurrentYear"
    {	  	
        VARIANT parm1, parm2, parm3, parm4;
        parm1.vt = VT_BSTR;
        parm1.bstrVal = SysAllocString(L"CurrentYear");
        parm2.vt = VT_BOOL;
        parm2.boolVal = false;
        parm3.vt = VT_I4;
        parm3.lVal = 1; //msoPropertyTypeNumber = 1
        parm4.vt = VT_I4;
        parm4.lVal = 1999;

        OLEMethod(DISPATCH_METHOD, NULL, pCustomProps, L"Add", 4, parm4,
            parm3, parm2, parm1);
        ::MessageBox(NULL, "Custom property added, examine document.",
            "Custom Property", 0x10000);
        SysFreeString(parm1.bstrVal);
    }

    // Get the custom property "CurrentYear" and delete it
    IDispatch *pCustomProp;
    {
        VARIANT result;
        VariantInit(&result);
        VARIANT x;
        x.vt = VT_BSTR;
        x.bstrVal = ::SysAllocString(L"CurrentYear");
        OLEMethod(DISPATCH_PROPERTYGET, &result, pCustomProps, L"Item",
            1, x);
        pCustomProp = result.pdispVal;
        SysFreeString(x.bstrVal);
        OLEMethod(DISPATCH_METHOD, NULL, pCustomProp, L"Delete", 0);
        ::MessageBox(NULL,
            "Custom property removed, examine document.",
            "Custom Property", 0x10000);
    }

    // Close the document without saving changes and quit Word
    {
        VARIANT x;
        x.vt = VT_BOOL;
        x.boolVal = false;
        OLEMethod(DISPATCH_METHOD, NULL, pDoc, L"Close", 1, x);
        OLEMethod(DISPATCH_METHOD, NULL, pWordApp, L"Quit", 0);
    }

    // Cleanup
    pCustomProp->Release();
    pCustomProps->Release();
    pPropSubject->Release();
    pProps->Release();
    pDoc->Release();
    pDocs->Release();
    pWordApp->Release();


    // Uninitialize COM for this thread...
    CoUninitialize();*/

bool performUnicodeConversion(const string_t& textFont, const wstring_t& text,
    wstring_t& textUnicode)
{
    if (textFont == "Arial Armenian") {
        textUnicode = L"Abrakadabra";
        return true;
    }

    return false;
}

int analyzeDoc(WordApp& word, const string_t& fileName, const string_t& outputFolder)
{
    Poco::Path p(fileName);
    p.makeAbsolute();
    wstring_t wideName = toUtf16(p.toString());

    tDocumentsSp docs = word.getDocuments();
    tDocumentSp doc   = docs->open(wideName);
    if (!doc)
        return 0;

    tSelectionSp s = word.getSelection();
    int totalCharsQty = s->allCharactersCount(), endPos = 0;

    std::set<string_t> usedFonts;
    std::stringstream ss;

    do {
        s->selectCurrentFont();
        tFontSp fnt = s->getFont();

        string_t textFont = fnt->getFaceName();
        wstring_t text = s->getSelectionText(), textUnicode;

        if ( performUnicodeConversion(textFont, text, textUnicode) )
            s->setSelectionText(textUnicode);

        ///fnt->setFaceName("Ubuntu Mono");

        endPos = s->getEndPos();
        s->setStartPos(endPos);

        usedFonts.insert(textFont);
    } while ( endPos < totalCharsQty - 1 );

    doc->saveAs(Poco::Path(outputFolder).makeAbsolute().makeDirectory().toString() 
        + p.getFileName());

    ss << "Fonts inside '" << fileName << "' are: \n";
    for (auto it = usedFonts.begin(); it != usedFonts.end(); ++it)
        ss << *it << std::endl;
    writeFileAsBinary("fonts_" + p.getBaseName() + ".txt", ss.str());

    return 0;
}

void analyzeWordDocs( const string_t& inputFolder, const string_t& outputFolder )
{
    WordApp word;
    word.setVisible(true);

    FileFinder ff(false, inputFolder, "docx");

    auto files = ff.getFiles();
    for (auto it = files.begin(); it != files.end(); ++it) {
        std::cout << analyzeDoc(word, ff.getRootPath() + *it, outputFolder);
    }
}
