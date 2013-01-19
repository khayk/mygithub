#pragma once

typedef std::wstring  wstring_t;
typedef std::string   string_t;

class Cmd;

typedef std::vector<char>      tCharBuf;
typedef std::vector<wchar_t>   tWCharBuf;
typedef std::vector<wstring_t> tWStrVec;
typedef boost::shared_ptr<Cmd> tCmdSp;
