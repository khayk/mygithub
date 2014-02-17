#ifndef WINDWORD_SELECTION_H__
#define WINDWORD_SELECTION_H__

#include "Typedefs.h"
#include "Font.h"

class Range;
typedef boost::shared_ptr<Range> tRangeSp;

class Selection {
public:
    enum MoveDirection {
        mdLeft = 1,
        mdRight,
        mdUp,
        mdDown
    };

public:
    Selection(IDispatch* selection);
    ~Selection();

public: /// methods

    /// return the number of all characters
    int  allCharactersCount() const;

    /// move the cursor forward/backward based on forward value
    /// select text while moving if the 'selectWhileMoving' is true
    /// returns the number of units it's been moved
    int  moveCursor(MoveDirection dir, bool selectWhileMoving = false);
    
    int  setStartPos(int newPos);
    int  setEndPos(int newPos);
    int  getStartPos() const;
    int  getEndPos() const;

    /// return string from the current position with specified length by
    /// selecting forward
    wstring_t getString(int length);

    /// get selected string
    wstring_t getSelectionText();
    tRangeSp  getFormattedText();

    /// change the text of current selection, no formatting will be changed
    void setSelectionText(const wstring_t& text);

    /// select whole document
    void select();
    void selectAll();
    
    void selectCurrentColor();
    void selectCurrentFont();
    void selectCurrentTabs();

    void copyFormat();
    void pasteFormat();

    tFontSp getFont();
//     void setRange(int startPos, int endPos);

public: /// properties

protected:
    IDispatch* s_;
};

typedef boost::shared_ptr<Selection> tSelectionSp;

#endif

