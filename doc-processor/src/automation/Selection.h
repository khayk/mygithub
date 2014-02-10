#ifndef WINDWORD_SELECTION_H__
#define WINDWORD_SELECTION_H__

#include "Typedefs.h"
#include "Font.h"

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

    /// move the cursor forward/backward based on forward value
    /// select text while moving if the 'selectWhileMoving' is true
    /// returns the number of units it's been moved
    int moveCursor(MoveDirection dir, bool selectWhileMoving = false);

    /// return string from the current position with specified length by
    /// selecting forward
    wstring_t getString(int length);

    /// get selected string
    wstring_t getSelectedString();

    /// return the quantity of characters
    int getCharactersQty() const;

    /// select whole document
    void selectAll();
    void selectCurrentFont();

    void setStartPos(int newPos);
    void setEndPos(int newPos);

    int  getStartPos() const;
    int  getEndPos() const;

    tFontSp getFont();

    //void closeActiveDocument(bool save = true);
    //void closeAll(bool save = true);

    /// set the font for the selected text
    //void setFont(const wstring_t& faceName);

public: /// properties

private:
    IDispatch* s_;
};

typedef boost::shared_ptr<Selection> tSelectionSp;

#endif

