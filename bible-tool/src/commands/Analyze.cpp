#include "StdAfx.h"
#include "Analyze.h"

Analyze::Analyze( const wstring_t& cmdName )
    : Cmd(cmdName)
{

}

void Analyze::initialize( wstring_t& cmdArgs )
{
    tWStrSet argNames;
    argNames.insert(ARG_METHOD);
    argNames.insert(ARG_SRC_FILE);
    argNames.insert(ARG_WORD);    

    if ( !extractArgumets(cmdArgs, argNames) )
        throw std::exception("command line parsing error");
}

void Analyze::doCommand()
{
    /// source file to analyze
    wstring_t srcFile = getArgValue(ARG_SRC_FILE);

    /// determine analyze method
    wstring_t method = getArgValue(ARG_METHOD);

    if ( method == L"word" ) {
        /// word to search
        wstring_t word = getArgValue(ARG_WORD);

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
