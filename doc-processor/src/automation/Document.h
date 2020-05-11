#ifndef WINDWORD_DOCUMENT_H
#define WINDWORD_DOCUMENT_H

#include "Typedefs.h"
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
    tFormFieldsSp getFormFields();

    tSectionsSp    getSections();

    tRangeSp       getContent();
    tStoryRangesSp getStoryRanges();
    tCharactersSp  getCharacters();
    tParagraphsSp  getParagraphs();
    tSentencesSp   getSentences();
    tStylesSp      getStyles();

private:
    IDispatch* doc_;
};

typedef std::shared_ptr<Document> tDocumentSp;
typedef std::weak_ptr<Document>   tDocumentWp;

#endif