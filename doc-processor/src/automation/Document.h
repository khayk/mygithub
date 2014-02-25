#ifndef WINDWORD_DOCUMENT_H
#define WINDWORD_DOCUMENT_H

#include "Typedefs.h"
#include <boost/weak_ptr.hpp>
#include "Range.h"

class Document
{
public:
    Document(IDispatch* doc);
    ~Document();

    void close();
    void save();
    void saveAs(const string_t& fullPath);

    //ADD_NOTES_INTERFACE(doc_);
    tFootnotesSp getFootnotes();

    tRangeSp       getContent();
    tStoryRangesSp getStoryRanges();
    tCharactersSp  getCharacters();
    tParagraphsSp  getParagraphs();

private:
    IDispatch* doc_;
};

typedef boost::shared_ptr<Document> tDocumentSp;
typedef boost::weak_ptr<Document>   tDocumentWp;

#endif