#ifndef OFFICE_INTEROP_WORD_H
#define OFFICE_INTEROP_WORD_H

#include "WordApp.h"

class OfficeInteropWord
{
public:
    OfficeInteropWord();
    ~OfficeInteropWord();

    tWordAppSp wordApp();

private:
    

    /// internal data representation
    tWordAppSp   app_;
    IDispatch*   wordApp_;
};

#endif