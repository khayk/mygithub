#include "StdAfx.h"
#include "Common.h"

#include <Poco/Path.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/SHA1Engine.h>
#include <Poco/DigestStream.h>
#include <Poco/StreamCopier.h>

#include <Windows.h>
#include <fstream>

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

string_t makeCentralizedNote( const std::string& note, const char ch, size_t len )
{
    if (len <= note.size() + 2)
        return note;

    std::string result(len, ch);
    std::string::size_type offset = (len - note.size()) / 2;
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

void reportFailure( const string_t& fnName, const string_t& params, HRESULT hr )
{
    char buf[256];

    sprintf_s(buf, 
        "%s(%s) failed. hr = 0x%08lx", fnName.c_str(), params.c_str(), hr);

    // TODO: later use logging
    ::MessageBox(NULL, buf,  "Error", 0x10010);
    throw std::exception(buf);
}
