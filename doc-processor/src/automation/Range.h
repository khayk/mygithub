#pragma once

#include "Typedefs.h"
#include "Selection.h"

class Range : public Selection
{
public:
    Range(IDispatch* range);
    ~Range();

    /// get selected string
//     wstring_t getText() const;
//     void setText(const wstring_t& text);

private:
    //IDispatch* range_;
};

typedef boost::shared_ptr<Range> tRangeSp;