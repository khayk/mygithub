#ifndef WORD_FIND_H
#define WORD_FIND_H

#include "Typedefs.h"
#include "BaseRS.h"

class Range;
typedef std::shared_ptr<Range> tRangeSp;

class Find : public BaseObject
{
public:
    Find(IDispatch* find);
    bool execute();
    void clearFormatting();

    tFontSp getFont() const;

    /// get selected string
    void setText(const wstring_t& text);
    wstring_t getText() const;
};

class Find;
typedef std::shared_ptr<Find> tFindSp;

#endif