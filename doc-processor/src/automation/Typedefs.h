#ifndef WORD_TYPEDEFS_H
#define WORD_TYPEDEFS_H

#include <ole2.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include "../utils/RefCounter.h"
#include "Enums.h"

typedef boost::intrusive_ptr<IDispatch> tDispatchIp;


#define SafeRelease(p)      if (p) { p->Release(); p = 0; }

#define Validation3(fnName, param, hr)                                         \
{                                                                              \
    if (FAILED(hr)) {                                                          \
        reportFailure(fnName, param, hr);                                      \
        return E_FAIL;                                                         \
    }                                                                          \
}

#define Validation2(fnName, hr)    Validation3(fnName, string_t(""), hr)


#endif