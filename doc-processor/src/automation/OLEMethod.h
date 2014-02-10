#ifndef OLE_METHOD_H
#define OLE_METHOD_H

#include "Typedefs.h"

int getPropertyInt(IDispatch* pDisp, LPOLESTR propName);
int setPropertyInt(IDispatch* pDisp, LPOLESTR propName, int value);

string_t   getPropStr(IDispatch* pDisp, LPOLESTR propName);
wstring_t  getPropWStr(IDispatch* pDisp, LPOLESTR propName);
IDispatch* getPropertyDispatch(IDispatch* pDisp, LPOLESTR propName);

HRESULT OLEMethod(int autoType, VARIANT *pvResult, IDispatch *pDisp, 
    LPOLESTR ptName, int cArgs...);

#endif