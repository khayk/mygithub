#pragma once

#include "Typedefs.h"

class Range
{
public:
    Range(IDispatch* range);
    ~Range();

    /// get selected string
    wstring_t getText() const;
    void setText(const wstring_t& text);

private:
    IDispatch* range_;
};

typedef boost::shared_ptr<Range> tRangeSp;