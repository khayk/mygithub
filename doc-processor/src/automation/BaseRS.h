#pragma once

#include "Typedefs.h"
#include "Font.h"

class Range;
typedef boost::shared_ptr<Range> tRangeSp;

class BaseRS {
public:
    BaseRS(IDispatch* base);
    ~BaseRS();

    IDispatch* getIDispatch() const;

    /// return the number of all characters
    int     getStoryLength() const;
    
    tFontSp getFont() const;
    void    setFont(const tFontSp& font);

    int  setStart(int pos);
    int  getStart() const;

    int  setEnd(int pos);
    int  getEnd() const;

    /// get selected string
    void setText(const wstring_t& text);
    wstring_t getText() const;
    
    void setFormattedText(const tRangeSp& range);
    tRangeSp  getFormattedText() const;

    /// select current object and whole document
    void select();
    void selectAll();

protected:
    IDispatch* base_;
};