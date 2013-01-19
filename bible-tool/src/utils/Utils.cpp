#include "StdAfx.h"
#include "Utils.h"

/// ----------------------------------------------------------------------------
wstring_t getAppDir() {
    wchar_t szAppPath[MAX_PATH] = L"";
    GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);

    wstring_t appDir(szAppPath);
    appDir.erase(appDir.find_last_of(L"\\"));
    return appDir;
}

/// ----------------------------------------------------------------------------
bool isAbsolutePath( const wstring_t& str )
{
    return ( str.find(L":") != wstring_t::npos );
}

/// ----------------------------------------------------------------------------
bool isNumber( const wstring_t& str )
{
	wstring_t::const_iterator it = str.begin();
	while (it != str.end() && iswdigit(*it))
		++it;
	return !str.empty() && it == str.end();
}

/// ----------------------------------------------------------------------------
string_t toUtf8( const wstring_t& source )
{
	string_t result;
	convertFormat(source, result, CP_UTF8);
	return result;
}

/// ----------------------------------------------------------------------------
wstring_t toUtf16( const string_t& source )
{
	wstring_t result;
	convertFormat(source, result, CP_ACP);
	return result;
}

/// ----------------------------------------------------------------------------
void convertFormat( const wstring_t& uniStr, string_t& multiStr, long codePage )
{
	long lenUni = (long) uniStr.length();
	long lenMulti = ::WideCharToMultiByte( codePage, 0, uniStr.c_str()
		, lenUni, 0, 0, NULL, NULL);

	if (lenMulti > 0)
	{
		multiStr.resize(lenMulti);
		lenMulti = ::WideCharToMultiByte( codePage, 0, uniStr.c_str(), lenUni
			, &multiStr[0], multiStr.size(), NULL, NULL);
	}
}

/// ----------------------------------------------------------------------------
void convertFormat( const string_t& multiStr, wstring_t& uniStr, long codePage )
{
	if ( multiStr.size() == 0 ) {
		uniStr.clear();
		return;
	}

	int lenUni = MultiByteToWideChar(codePage, 0, multiStr.c_str(), 
		multiStr.size(), 0, 0);

	if ( lenUni > 0 ) {
		uniStr.resize(lenUni);
		lenUni = MultiByteToWideChar(codePage, 0, multiStr.c_str(), 
			multiStr.size(), &uniStr[0], uniStr.size());
	}
}

/// ----------------------------------------------------------------------------
bool readFileAsBinary( const wstring_t& fileName, string_t& rawData )
{
	wstring_t appDir = getAppDir();
	wstring_t fullPath = fileName;
	if (!isAbsolutePath(fileName))
		fullPath = appDir + L"\\" + fileName;

	std::ifstream srcFile(fullPath, std::ios::in | std::ios::binary);
	if ( !srcFile ) {
		std::wcout << L"failed to open input file: " << fullPath << std::endl;
		return false;
	}

	srcFile.seekg(0, std::ios_base::end);
	size_t size = static_cast<size_t>(srcFile.tellg());
	srcFile.seekg(0, std::ios_base::beg);

	if ( size > 0 ) {
		rawData.resize(size);
		srcFile.read(&rawData[0], size);
	}

	srcFile.close();
	return true;
}

/// ----------------------------------------------------------------------------
bool writeFileAsBinary( const wstring_t& fileName, string_t& rawData )
{
	wstring_t appDir = getAppDir();
	wstring_t fullPath = fileName;
	if (!isAbsolutePath(fileName))
		fullPath = appDir + L"\\" + fileName;

	std::ofstream destFile(fullPath, std::ios::out | std::ios::binary);
	if ( !destFile ) {
		std::wcout << L"failed to open destination file: " << fullPath << std::endl;
		return false;
	}

	/// write into destination file
	destFile.write(&rawData[0], rawData.size());
	destFile.close();

	return true;
}

