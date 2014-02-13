#include "StdAfx.h"
#include "Documents.h"
#include "Document.h"
#include "OLEMethod.h"
#include "../RootLogger.h"

Documents::Documents(IDispatch* docs)
    : docs_(docs)
{

}

Documents::~Documents()
{
    docsList.clear();
    SafeRelease(docs_);
}

tDocumentSp Documents::open( const wstring_t& docName )
{
    VARIANT result;
    VariantInit(&result);
    VARIANT fname;
    fname.vt = VT_BSTR;
    fname.bstrVal = ::SysAllocString(docName.c_str());

    try {
        OLEMethod(DISPATCH_METHOD, &result, docs_, L"Open", 1, fname);
    }
    catch (const std::exception& e) {
        logError(Poco::Logger::root(), string_t("error: ") + e.what());
        SysFreeString(fname.bstrVal);
        return tDocumentSp();
    }
    

    tDocumentSp doc(new Document(result.pdispVal));
    docsList.push_back(doc);
    
    SysFreeString(fname.bstrVal);
    return doc;
}

void Documents::closeAll()
{
    for (auto it = docsList.begin(); it != docsList.end(); ++it) {
        tDocumentSp sp = it->lock();
        if (sp)
            sp->close();
    }
    docsList.clear();
}
  
