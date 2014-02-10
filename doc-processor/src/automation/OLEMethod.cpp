#include "StdAfx.h"
#include "OLEMethod.h"
#include "../utils/Common.h"

#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>

HRESULT OLEMethod(int autoType, VARIANT *pvResult, IDispatch *pDisp, 
    LPOLESTR ptName, int cArgs...) 
{
    // Begin variable-argument list...
    va_list marker;
    va_start(marker, cArgs);

    if (!pDisp) {
        reportFailure("OLEMethod", "NULL IDispatch", E_FAIL);
        return E_FAIL;
    }

    // Variables used...
    DISPPARAMS dp = { NULL, NULL, 0, 0 };
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    DISPID dispID;
    HRESULT hr;

    char szName[256];

    // Convert down to ANSI
    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

    // Get DISPID for name passed...
    hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, 
        &dispID);
    if (FAILED(hr)) {
        reportFailure("IDispatch::GetIDsOfNames", szName, hr);
        return hr;
    }

    // Allocate memory for arguments...
    boost::scoped_array<VARIANT> pArgs(new VARIANT[cArgs+1]);

    // Extract arguments...
    for(int i=0; i<cArgs; i++) {
        pArgs[i] = va_arg(marker, VARIANT);
    }

    // Build DISPPARAMS
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs.get();

    // Handle special-case for property-puts!
    if (autoType & DISPATCH_PROPERTYPUT) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }

    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, 
        &dp, pvResult, NULL, NULL);
    if (FAILED(hr)) {
        reportFailure("IDispatch::Invoke", string_t("Name:") + szName 
            + " dispId: " + boost::lexical_cast<string_t>(dispID), hr);
        return hr;
    }

    // End variable-argument section...
    va_end(marker);
    return hr;

}

int getPropertyInt( IDispatch* pDisp, LPOLESTR propName )
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, pDisp, propName, 0);
    return result.intVal;
}

int setPropertyInt( IDispatch* pDisp, LPOLESTR propName, int value )
{
    VARIANT variant;
    VARIANT result;

    VariantInit(&variant);
    VariantInit(&result);

    variant.vt =VT_I4;
    variant.lVal = value;
    OLEMethod(DISPATCH_PROPERTYPUT, &result, pDisp, propName, 1, variant);
    return result.intVal;
}

IDispatch* getPropertyDispatch( IDispatch* pDisp, LPOLESTR propName )
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, pDisp, propName, 0);
    return result.pdispVal;
}

string_t getPropStr( IDispatch* pDisp, LPOLESTR propName )
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, pDisp, propName, 0);
    return toUtf8(result.bstrVal);
}

wstring_t getPropWStr( IDispatch* pDisp, LPOLESTR propName )
{
    VARIANT result;
    VariantInit(&result);
    OLEMethod(DISPATCH_PROPERTYGET, &result, pDisp, propName, 0);
    return result.bstrVal;
}
