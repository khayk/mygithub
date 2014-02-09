#ifndef MS_WORD_H__ 
#define MS_WORD_H__

#include "../utils/RefCounter.h"

class  MSWord {
public:
    enum MoveDirection {
        mdLeft = 1,
        mdRight,
        mdUp,
        mdDown
    };

public:
    MSWord();
    ~MSWord();

    void open(const wstring_t& file, bool visible = true);
    void closeActiveDocument(bool save = true);
    void closeAll(bool save = true);

    /// closes winword.exe process
    void quit();

    /// change the visibility of the Microsoft Word application
    void setVisible(bool visible = true);

    /// set the font for the selected text
    void setFont(const wstring_t& faceName);

    /// move the cursor forward/backward based on forward value
    /// select text while moving if the 'selectWhileMoving' is true
    void moveCursor(MoveDirection dir, bool selectWhileMoving = false);

    /// return string from the current position with specified length by
    /// selecting forward
    wstring_t getString(int length);

    /// get selected string
    wstring_t getSelectedString();

    /// select whole document
    void selectAll();

protected:
    HRESULT initialize(bool visible = true);

    /// some helper functions and types
    typedef boost::intrusive_ptr<IDispatch> tDispatchIp;
    tDispatchIp getSelection();

    tDispatchIp selection_;

private:
    /// internal data representation
    IDispatch* wordApp_;
    IDispatch* activeDoc_;
    IDispatch* docs_;
    IDispatch* app_;

    HRESULT		hr_;
};
#endif