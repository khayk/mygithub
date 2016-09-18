#ifndef EXCEL_FILE_H
#define EXCEL_FILE_H

#include <ole2.h>
#include "../automation/Range.h"
#include <memory>
#include <string>
#include <list>

// BEGIN_OBJECT(Cell, BaseObject)
// END_OBJECT(Cell)

class ExcelRange;
typedef std::shared_ptr<ExcelRange> tExcelRangeSp;

BEGIN_OBJECT(ExcelRange, BaseObject)
public:
	int numRows();
	int numCols();
	int count();

	//tExcelRangeSp cells();

// 	void select();
// 	void activate();
// 	void show();

	tExcelRangeSp item(int itemNumber);
	tExcelRangeSp rows();
	tExcelRangeSp columns();
	tExcelRangeSp next();
	tExcelRangeSp previous();

	tFontSp getFont() const;
	void setFont(const tFontSp& font);

	std::wstring getValue2();
	void setValue2(const std::wstring& text);

	std::wstring getText();
	void setText(const std::wstring& text);
END_OBJECT(ExcelRange)


// BEGIN_OBJECT(Rows, BaseObject)
// public:
// 	tRangeSp getRange();
// END_OBJECT(Rows)


BEGIN_OBJECT(Worksheet, BaseObject)
public:
	void activate();
	tExcelRangeSp usedRange();
 
// 	tCellSp        getCell(int r, int c);
// 	const tCellSp& getCell(int r, int c) const;
// 
// 	void setCell(int r, int c, const tCellSp& cp);
END_OBJECT(Worksheet);


BEGIN_OBJECT(Worksheets, BaseObject)
public:
	int getCount();
	tWorksheetSp getItem(int index);
END_OBJECT(Worksheets);


BEGIN_OBJECT(Workbook, BaseObject)
public:
	tWorksheetsSp worksheets();

	void close();
	void save();
	void saveAs(const std::string& fullPath);
END_OBJECT(Workbook);


typedef std::weak_ptr<Workbook> tWorkbookWp;


BEGIN_OBJECT(Workbooks, BaseObject)
public:
	~Workbooks();

	tWorkbookSp open(const std::wstring& docName);
	void closeAll();
private:
	std::list<tWorkbookSp> wbs_;
END_OBJECT(Workbooks)


class ExcelApp {
public:
    ExcelApp();
    ~ExcelApp();

public:
	void quit();
	void setVisible(bool visibility);

	tWorkbooksSp getWorkbooks();

private:
	IDispatch* inst_;
	tWorkbooksSp wbooks_;
};

typedef std::shared_ptr<ExcelApp> tExcelAppSp;

#endif // EXCEL_FILE_H
