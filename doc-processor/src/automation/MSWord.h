#ifndef MS_WORD_H__ 
#define MS_WORD_H__

#include "../utils/RefCounter.h"

class  MSWord {
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


protected:
    HRESULT initialize(bool visible = true);

    IDispatch* wordApp_;
    IDispatch* activeDoc_;
    IDispatch* docs_;
    IDispatch* app_;

    HRESULT		hr_;
};
#endif