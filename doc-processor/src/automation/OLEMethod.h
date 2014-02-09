#ifndef OLE_METHOD_H__ 
#define OLE_METHOD_H__

#include <ole2.h>

HRESULT OLEMethod(int autoType, VARIANT *pvResult, IDispatch *pDisp, 
    LPOLESTR ptName, int cArgs...);

#endif