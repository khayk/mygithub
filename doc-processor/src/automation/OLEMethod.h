#ifndef OLE_METHOD_H
#define OLE_METHOD_H

#include "Typedefs.h"

float getPropertyFloat(IDispatch* disp, LPOLESTR propName);

int getPropertyInt(IDispatch* disp, LPOLESTR propName);
int setPropertyInt(IDispatch* disp, LPOLESTR propName, int val);

wstring_t  getPropStr(IDispatch* disp, LPOLESTR propName);
void       setPropStr(IDispatch* disp, LPOLESTR propName, const wstring_t& val);

IDispatch* getPropertyDispatch(IDispatch* disp, LPOLESTR propName);
void setPropertyDispatch(IDispatch* dispFor, IDispatch* dispFrom, LPOLESTR propName);

HRESULT OLEMethod(int autoType, VARIANT *pvResult, IDispatch *disp, 
    LPOLESTR ptName, int cArgs...);

#endif