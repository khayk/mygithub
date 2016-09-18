#include "stdafx.h"
#include "../automation/OLEMethod.h"
#include "ExcelApp.h"
#include "../RootLogger.h"


ExcelApp::ExcelApp()
{
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);
	Validation2("CLSIDFromProgID", hr);

	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER,
		IID_IDispatch, (void **)& inst_);
	Validation2("CoCreateInstance", hr);
	setVisible(false);
}

ExcelApp::~ExcelApp()
{
	quit();
}

void ExcelApp::quit()
{
	if (!inst_)
		return;

	if (wbooks_) {
 		wbooks_->closeAll();
		wbooks_.reset();
	}

	OLEMethod(DISPATCH_METHOD, NULL, inst_, L"Quit", 0);
	SafeRelease(inst_);
}

void ExcelApp::setVisible(bool visibility)
{
	VARIANT x;
	x.vt = VT_I4;
	x.lVal = visibility;
	OLEMethod(DISPATCH_PROPERTYPUT, NULL, inst_, L"Visible", 1, x);
}

tWorkbooksSp ExcelApp::getWorkbooks()
{
	if (!wbooks_) {
		VARIANT result;
		VariantInit(&result);
		HRESULT hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, inst_, L"Workbooks", 0);
		wbooks_.reset(new Workbooks(result.pdispVal));
	}
	return wbooks_;
}


Workbooks::~Workbooks()
{
	closeAll();
}

tWorkbookSp Workbooks::open(const std::wstring& docName)
{
	VARIANT result;
	VariantInit(&result);
	VARIANT fname;
	fname.vt = VT_BSTR;
	fname.bstrVal = ::SysAllocString(docName.c_str());

	try {
		OLEMethod(DISPATCH_METHOD, &result, disp_, L"Open", 1, fname);
	}
	catch (const std::exception& e) {
		logError(Poco::Logger::root(), std::string("error: ") + e.what());
		SysFreeString(fname.bstrVal);
		return tWorkbookSp();
	}


	tWorkbookSp wb(new Workbook(result.pdispVal));
	wbs_.push_back(wb);

	SysFreeString(fname.bstrVal);
	return wb;
}

void Workbooks::closeAll()
{
	for (auto it = wbs_.begin(); it != wbs_.end(); ++it) {
		tWorkbookSp wb = (*it);// .lock();
		if (wb)
			wb->close();
	}
	wbs_.clear();
}

void Workbook::close()
{
	if (disp_) {
		VARIANT saveChanges;

		VariantInit(&saveChanges);

		saveChanges.vt = VT_I4;
		saveChanges.intVal = 0;

		OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Close", 1, saveChanges);
		SafeRelease(disp_);
	}
}

tWorksheetsSp Workbook::worksheets()
{
	if (disp_) {
		VARIANT result;
		VariantInit(&result);
		HRESULT hr_ = OLEMethod(DISPATCH_PROPERTYGET, &result, disp_, L"Worksheets", 0);
		return std::make_shared<Worksheets>(result.pdispVal);
	}
	return tWorksheetsSp();
}

void Workbook::saveAs(const std::string& fullPath)
{
	if (disp_) {
		VARIANT fname;
		VariantInit(&fname);
		fname.vt = VT_BSTR;
		fname.bstrVal = ::SysAllocString(toUtf16(fullPath).c_str());

		OLEMethod(DISPATCH_METHOD, NULL, disp_, L"SaveAs", 1, fname);
		SysFreeString(fname.bstrVal);
	}
}

int Worksheets::getCount()
{
	return getPropertyInt(disp_, L"Count");
}

tWorksheetSp Worksheets::getItem(int index)
{
	VARIANT result;
	VariantInit(&result);

	VARIANT x;
	x.vt = VT_I4;
	x.intVal = index;
	OLEMethod(DISPATCH_PROPERTYGET, &result, disp_, L"Item", 1, x);

	return std::make_shared<Worksheet>(result.pdispVal);
}

void Worksheet::activate()
{
	OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Activate", 0);
}

tExcelRangeSp Worksheet::usedRange()
{
	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"UsedRange")));
}

int ExcelRange::numRows()
{
	tExcelRangeSp rs = rows();
	return getPropertyInt(rs->disp_, L"Count");
}

int ExcelRange::numCols()
{
	tExcelRangeSp rs = columns();
	return getPropertyInt(rs->disp_, L"Count");
}

int ExcelRange::count()
{
	return getPropertyInt(disp_, L"Count");
}

tExcelRangeSp ExcelRange::rows()
{
	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"Rows")));
}

tExcelRangeSp ExcelRange::columns()
{
	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"Columns")));
}

tExcelRangeSp ExcelRange::next()
{
	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"Next")));
}

// tExcelRangeSp ExcelRange::cells()
// {
// 	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"Cells")));
// }

tExcelRangeSp ExcelRange::previous()
{
	return tExcelRangeSp(new ExcelRange(getPropertyDispatch(disp_, L"Previous")));
}

std::wstring ExcelRange::getText()
{
	return getPropStr(disp_, L"Text");
}

void ExcelRange::setText(const std::wstring& text)
{
	setPropStr(disp_, L"Text", text);
}

tFontSp ExcelRange::getFont() const
{
	return tFontSp(new Font(getPropertyDispatch(disp_, L"Font")));
}

void ExcelRange::setFont(const tFontSp& font)
{
	setPropertyDispatch(disp_, font->getIDispatch(), L"Font");
}

tExcelRangeSp ExcelRange::item(int rowIndex)
{
	VARIANT variant;
	VARIANT result;

	VariantInit(&variant);
	VariantInit(&result);

	variant.vt = VT_I4;
	variant.intVal = rowIndex;
	OLEMethod(DISPATCH_PROPERTYGET, &result, disp_, L"Item", 1, variant);
	return std::make_shared<ExcelRange>(result.pdispVal);
}

std::wstring ExcelRange::getValue2()
{
	wstring_t sr;
	VARIANT v = getPropVariant(disp_, L"Value2");
	if (v.vt == VT_BSTR) {
		sr = v.bstrVal;
		VariantClear(&v);
	}
	return sr;
}

void ExcelRange::setValue2(const std::wstring& text)
{
	VARIANT variant;
	VariantInit(&variant);

	variant.vt = VT_BSTR;
	variant.bstrVal = ::SysAllocString(text.c_str());
	setPropVariant(disp_, L"Value2", variant);
	SysFreeString(variant.bstrVal);
}

// void ExcelRange::activate()
// {
// 	OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Activate", 0);
// }
// 
// void ExcelRange::select()
// {
// 	OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Select", 0);
// }
// 
// void ExcelRange::show()
// {
// 	OLEMethod(DISPATCH_METHOD, NULL, disp_, L"Show", 0);
// }
