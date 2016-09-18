#ifndef WINDWORD_SELECTION_H__
#define WINDWORD_SELECTION_H__

#include "BaseRS.h"

class Selection : public BaseRS {
public:
    Selection(IDispatch* selection);

public: /// methods

    /// move the cursor forward/backward based on forward value
    /// select text while moving if the 'selectWhileMoving' is true
    /// returns the number of units it's been moved
    int  moveCursor(MoveDirection dir, bool selectWhileMoving = false);
    
    /// return string from the current position with specified length by
    /// selecting forward
    /// wstring_t getString(int length); 
    
    void selectCurrentColor();
    void selectCurrentFont();

    void copyFormat();
    void pasteFormat();
};

typedef std::shared_ptr<Selection> tSelectionSp;

#endif

