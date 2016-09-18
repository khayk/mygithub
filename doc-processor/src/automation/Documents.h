#ifndef WINWORD_DOCUMENTS_H
#define WINWORD_DOCUMENTS_H

#include "Document.h"
#include <list>

class Documents
{
public:
    Documents(IDispatch* docs);
    ~Documents();
    
    tDocumentSp open(const std::wstring& docName);
    void closeAll();

private:
    IDispatch* docs_;
    std::list<tDocumentWp> docsList;
};

typedef std::shared_ptr<Documents> tDocumentsSp;

#endif