#ifndef OLE_METHOD_H
#define OLE_METHOD_H

#include "Typedefs.h"

float getPropertyFloat(IDispatch* disp, LPOLESTR propName);
void  setPropertyFloat(IDispatch* disp, LPOLESTR propName, float val);

int getPropertyInt(IDispatch* disp, LPOLESTR propName);
int setPropertyInt(IDispatch* disp, LPOLESTR propName, int val);

short getPropertyBool(IDispatch* disp, LPOLESTR propName);
void setPropertyBool(IDispatch* disp, LPOLESTR propName, short val);

wstring_t  getPropStr(IDispatch* disp, LPOLESTR propName);
void       setPropStr(IDispatch* disp, LPOLESTR propName, const wstring_t& val);

VARIANT  getPropVariant(IDispatch* disp, LPOLESTR propName);
void setPropVariant(IDispatch* disp, LPOLESTR propName, const VARIANT& val);

IDispatch* getPropertyDispatch(IDispatch* disp, LPOLESTR propName);
void setPropertyDispatch(IDispatch* dispFor, IDispatch* dispFrom, LPOLESTR propName);

HRESULT OLEMethod(int autoType, VARIANT *pvResult, IDispatch *disp, 
    LPOLESTR ptName, int cArgs...);

#endif