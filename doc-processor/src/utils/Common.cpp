#include "StdAfx.h"
#include "Common.h"

#include <Poco/Path.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/SHA1Engine.h>
#include <Poco/DigestStream.h>
#include <Poco/StreamCopier.h>

#include <ShlObj.h>
#include <Windows.h>
#include <fstream>
#include <sstream>

#include <boost/scoped_array.hpp>

using Poco::SHA1Engine;
using Poco::DigestEngine;
using Poco::StreamCopier;
using Poco::DigestOutputStream;

string_t toUtf8( const wstring_t& source )
{
    string_t result;
    Poco::UnicodeConverter::toUTF8(source, result);
    return result;
}

wstring_t toUtf16( const string_t& source )
{
    wstring_t result;
    Poco::UnicodeConverter::toUTF16(source, result);
    return result;
}

unsigned long getFileVersion( const wstring_t& fileName, wstring_t& fileVersion )
{
    DWORD handle;
    DWORD sz = GetFileVersionInfoSizeW(fileName.c_str(), &handle);

    int major1 = 0;
    int minor1 = 0;
    int major2 = 0;
    int minor2 = 0;

    if (sz) {
        boost::scoped_array<char> dataBuffer(new char[sz+1]);
        memset(dataBuffer.get(), 0, sz + 1);
        UINT itemLength = 0;
        LPBYTE tempBuffer = 0;

        if (!GetFileVersionInfoW(fileName.c_str(), handle, sz, dataBuffer.get()))
            return GetLastError();

        if (!VerQueryValueW(dataBuffer.get(), L"\\", (VOID FAR* FAR*)&tempBuffer,
            &itemLength))
            return GetLastError();

        if (itemLength == 0)
            return GetLastError();

        VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)tempBuffer;
        if (verInfo->dwSignature != 0xfeef04bd)
            return GetLastError();

        /// now everything is ok, we can extract version information
        major1 = HIWORD(verInfo->dwFileVersionMS);
        minor1 = LOWORD(verInfo->dwFileVersionMS);
        major2 = HIWORD(verInfo->dwFileVersionLS);
        minor2 = LOWORD(verInfo->dwFileVersionLS);
    }
    else {
        return GetLastError();
    }

    wchar_t intBuffer[11] = {0};
    _itow_s(major1, intBuffer, 10);
    fileVersion += intBuffer;
    fileVersion += L".";
    _itow_s(minor1, intBuffer, 10);
    fileVersion += intBuffer;
    fileVersion += L".";
    _itow_s(major2, intBuffer, 10);
    fileVersion += intBuffer;
    fileVersion += L".";
    _itow_s(minor2, intBuffer, 10);
    fileVersion += intBuffer;
    return 0;
}

string_t makeCentralizedNote( const string_t& note, const char ch, size_t len )
{
    if (len <= note.size() + 2)
        return note;

    string_t result(len, ch);
    string_t::size_type offset = (len - note.size()) / 2;
    result.replace(offset - 1, 1, " ");
    result.replace(offset, note.size(), note);
    result.replace(offset + note.size(), 1, " ");
    return result;
}

/// ----------------------------------------------------------------------------
void readFileAsBinary( const string_t& fileName, string_t& rawData )
{
    std::ifstream srcFile(toUtf16(fileName), std::ios::in | std::ios::binary);
    if ( !srcFile ) {
        throw std::exception(("Failed to open input file: " + fileName).c_str());
    }

    srcFile.seekg(0, std::ios_base::end);
    size_t size = static_cast<size_t>(srcFile.tellg());
    srcFile.seekg(0, std::ios_base::beg);

    if ( size > 0 ) {
        rawData.resize(size);
        srcFile.read(&rawData[0], size);
    }

    srcFile.close();
}

/// ----------------------------------------------------------------------------
void writeFileAsBinary( const string_t& fileName, const string_t& rawData )
{
    std::ofstream destFile(toUtf16(fileName), std::ios::out | std::ios::binary);
    if ( !destFile ) {
        throw std::exception(("Failed to open destination file: " + fileName).c_str());
    }

    /// write into destination file
    if (rawData.size() > 0)
        destFile.write(&rawData[0], rawData.size());
    destFile.close();
}

/// ----------------------------------------------------------------------------
string_t buildRelativePath(const string_t& thisPath, const string_t& relativeTo)
{
    if (thisPath == relativeTo)
        return "./";

    std::pair<string_t::const_iterator, string_t::const_iterator> mypair;

    mypair = mismatch(thisPath.begin(), thisPath.end(), relativeTo.begin());
    if (mypair.first != thisPath.end() && mypair.first != thisPath.begin()) {
        string_t::size_type offset = 0, pos = 0;
        offset = (mypair.first - thisPath.begin());
        pos    = thisPath.substr(0, offset).find_last_of("\\/");

        string_t tmpThis = thisPath.substr(pos + 1);
        string_t tmpRel  = relativeTo.substr(pos + 1);

        Poco::Path pathRel(tmpRel);
        pathRel.makeDirectory();
        int depth = pathRel.depth();
        string_t result;

        for (int i = 0; i < depth; ++i)
            result += "../";
        result += tmpThis;
        return result;
    }

    /// if it is not possible to build a relative path
    return thisPath;
}

void calculateSHA1( const string_t& file, string_t& sha1Value )
{
    std::ifstream istr(toUtf16(file), std::ios::binary);
    if (!istr) {
        throw std::exception(("Failed to open file '" + file + "'").c_str());
    }

    SHA1Engine sha;
    DigestOutputStream dos(sha);
    StreamCopier::copyStream(istr, dos);
    dos.close();
    sha1Value = DigestEngine::digestToHex(sha.digest());
}

void calculateSHA1_Str( const string_t& srcValue, string_t& sha1Value )
{
    std::stringstream ss(srcValue);

    SHA1Engine sha;
    DigestOutputStream dos(sha);
    StreamCopier::copyStream(ss, dos);
    dos.close();
    sha1Value = DigestEngine::digestToHex(sha.digest());
}

void reportFailure( const string_t& fnName, const string_t& params, HRESULT hr )
{
    char buf[256];

    sprintf_s(buf, 
        "%s(%s) failed. hr = 0x%08lx", fnName.c_str(), params.c_str(), hr);

    // TODO: later use logging
    //::MessageBoxA(NULL, buf,  "Error", 0x10010);
    throw std::exception(buf);
}

void replaceLineEndings( string_t& str )
{
    for (size_t i = 0; i < str.size(); ++i)
        if (str[i] == (char)13)
            str[i] = (char)10;
}

void replaceLineEndings( wstring_t& str )
{
    for (size_t i = 0; i < str.size(); ++i)
        if (str[i] == (char)13)
            str[i] = (char)10;
}

string_t getAppData( const string_t& vendorName, const string_t& applicationName )
{
    TCHAR wpath[MAX_PATH];
    HRESULT rc = SHGetFolderPath(
        NULL,
        CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE,
        NULL, SHGFP_TYPE_CURRENT,
        wpath);

    if (SUCCEEDED(rc)) {
        string_t localAppData;
        localAppData = toUtf8(wpath);
        Poco::Path path(localAppData);
        path.makeDirectory();
        if (!vendorName.empty())
            path.pushDirectory(vendorName);
        if (!applicationName.empty())
            path.pushDirectory(applicationName);
        return path.toString();
    }
    else {
        Poco::Path path(Poco::Path::home());
        path.pushDirectory("." + vendorName);
        path.pushDirectory(applicationName);
        return path.toString();
    }
}
