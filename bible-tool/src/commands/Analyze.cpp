#include "StdAfx.h"
#include "Analyze.h"

#define ANALYZE_METHOD   L"--method"
#define ANALYZE_WORD     L"--word"
#define ANALYZE_SRC_FILE L"--src-file"

Analyze::Analyze( const wstring_t& cmdName )
    : Cmd(cmdName)
{

}

void Analyze::initialize( wstring_t& cmdArgs )
{
    tWStrVec argNames;
    argNames.push_back(ANALYZE_METHOD);
    argNames.push_back(ANALYZE_SRC_FILE);
    argNames.push_back(ANALYZE_WORD);    

    if ( !extractArgumets(cmdArgs, argNames) )
        throw std::exception("command will not be executed");
}

void Analyze::doCommand()
{
    /// source file to analyze
    wstring_t srcFile = getArgValue(ANALYZE_SRC_FILE);

    /// determine analyze method
    wstring_t method = getArgValue(ANALYZE_METHOD);

    if ( method == L"word" ) {
        /// word to search
        wstring_t word = getArgValue(ANALYZE_WORD);

        string_t rawData;
        if ( !readFileAsBinary(srcFile, rawData) )
            return;

        /// make wide char string
        wstring_t uniStr;
        convertFormat(rawData, uniStr, CP_UTF8);

        wstring_t res;
        std::wstringstream wss(uniStr);
        while (!wss.eof()) {
            wstring_t line;
            getline(wss, line, wchar_t(13));

            if ( line.find(word) == wstring_t::npos )
                continue;

            wstring_t::size_type p = line.find(':');
            p = line.find(' ', p);

            res += line.substr(0, p);
            res += 13;
            res += 10;
        }

        convertFormat(res, rawData, CP_UTF8);

        wstring_t fileName = getFileName(srcFile);
        wstring_t fileExt  = getFileExt(srcFile);
        wstring_t newFile = L"res\\" + fileName  + L"_" + word + L"." + fileExt;
        writeFileAsBinary(newFile, rawData);
    }
}

void Analyze::showHelp()
{

}
