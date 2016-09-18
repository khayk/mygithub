#ifndef WORD_TYPEDEFS_H
#define WORD_TYPEDEFS_H

#include <ole2.h>
#include <boost/intrusive_ptr.hpp>
#include "../utils/RefCounter.h"
#include "../utils/Common.h"
#include "Enums.h"
#include <memory>
#include <string>

typedef boost::intrusive_ptr<IDispatch> tDispatchIp;


#define SafeRelease(p)      if (p) { p->Release(); p = 0; }

#define Validation3(fnName, param, hr)                                         \
{                                                                              \
    if (FAILED(hr)) {                                                          \
        reportFailure(fnName, param, hr);                                      \
        return;                                                                \
    }                                                                          \
}

#define Validation2(fnName, hr)    Validation3(fnName, std::string(""), hr)

#endif