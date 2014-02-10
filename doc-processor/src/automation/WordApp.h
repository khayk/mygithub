#ifndef MS_WORD_H__ 
#define MS_WORD_H__

#include "Selection.h"
#include "Documents.h"

class  WordApp {
public:
    WordApp();
    ~WordApp();

public:   /// interfaces
    tDocumentsSp getDocuments();

    /// returns an active document in the application
    tDocumentSp  getActiveDocument();

    /// Represents the current selection in a window or pane.
    tSelectionSp getSelection();

public:   /// Methods

    void quit();
    void setVisible(bool visible);

protected:
    HRESULT initialize( bool visible = true );

private:
    IDispatch* wordApp_;
    tDocumentsSp  docs_;
    HRESULT		hr_;
};

typedef boost::shared_ptr<WordApp> tWordAppSp;

#endif