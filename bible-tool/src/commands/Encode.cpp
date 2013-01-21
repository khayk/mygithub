#include "StdAfx.h"
#include "Encode.h"

#define ENC_SRC_ENCODING L"--src-encoding"
#define ENC_SRC_DIR      L"--src-dir"
#define ENC_DST_DIR      L"--dst-dir"

/// -----------------------------------------------------------------------
Encode::Encode( const wstring_t& cmdName )
	: Cmd(cmdName)
	, cpNumber_(0)
{

}

/// -----------------------------------------------------------------------
void Encode::initialize( wstring_t& cmdArgs )
{
	tWStrVec argNames;
	argNames.push_back(ENC_SRC_ENCODING);
	argNames.push_back(ENC_SRC_DIR);
	argNames.push_back(ENC_DST_DIR);
	
	if ( !extractArgumets(cmdArgs, argNames) )
		throw std::exception("command will not be executed");
}

/// -----------------------------------------------------------------------
void Encode::doCommand()
{
    wstring_t appDir = getAppDir();

    /// source directory
    wstring_t srcDir = getArgValue(ENC_SRC_DIR);
    if (!isAbsolutePath(srcDir))
        srcDir = appDir + L"\\" + srcDir;

    /// destination directory
    wstring_t dstDir = getArgValue(ENC_DST_DIR);
    if (!isAbsolutePath(dstDir))
        dstDir = appDir + L"\\" + dstDir;

    /// determine encoding
    wstring_t srcEncoding = getArgValue(ENC_SRC_ENCODING);
	if ( isNumber(srcEncoding) ) {
		cpNumber_ = toNumber<unsigned long>(srcEncoding);
	}

	/// create destination directories
	namespace fs = boost::filesystem;
	fs::path destPath(dstDir);
	if ( !fs::exists(destPath) ) {
		fs::create_directories(destPath);
	}

	/// start source directory scanning process
	fs::path someDir(srcDir);
	fs::directory_iterator end_iter;
	if ( fs::exists(someDir) && fs::is_directory(someDir)) {
		for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter) {
			if (fs::is_regular_file(dir_iter->status()) ) {
				fs::path curFile = *dir_iter;

				/// open input file
                string_t asciiBuff, utf8Buff;
                if ( !readFileAsBinary(curFile.wstring(), asciiBuff) )
                    continue;

				/// perform conversion
				if ( cpNumber_ > 0 )
					performConvertion(cpNumber_, asciiBuff, utf8Buff);
				else
					performConvertion(srcEncoding, asciiBuff, utf8Buff);				

				/// conversion succeeded
				if ( utf8Buff.size() > 0 ) {
					/// construct destination file name
					wstring_t newFile = dstDir + L"\\";
					newFile += curFile.filename().wstring();
                    writeFileAsBinary(newFile, utf8Buff);
				}
			}
		}
	}
}

/// -----------------------------------------------------------------------
void Encode::showHelp()
{
	std::wcout << L"usage: "
		<< cmd_ << L" " << ENC_SRC_ENCODING << L"=1251 " 
		<< ENC_SRC_DIR << L"=abs or relative directory "
		<< ENC_DST_DIR << L"=abs or relative directory"
		<< std::endl;
}

/// -----------------------------------------------------------------------
void Encode::loadCharactersMapping( const wstring_t& fileName )
{
	string_t rawData;
	if ( !readFileAsBinary(fileName, rawData) ) {
		return;
	}

	charMapping_.resize(256);

	wstring_t uniStr;
	convertFormat(rawData, uniStr, CP_UTF8);

	std::wstringstream wss(uniStr);
	
	while (!wss.eof()) {
		wstring_t line;
		getline(wss, line);

		wchar_t chFrom = 0, chTo = 0;
		std::wstringstream wssl(line);
		wssl >> chFrom >> chTo;
		if ( chFrom != 0 || chTo != 0 ) {
			if ( chFrom < 256 ) {
				charMapping_[chFrom] = chTo;
				if (charMappingMap_.find(chFrom) == charMappingMap_.end()) {
					charMappingMap_.insert( std::make_pair(chFrom, chTo) );
				}
				else {
					std::cout << "invalid mapping file, see symbol: " 
						<< (int)chFrom << " - " << chFrom << std::endl;
				}
			}
			else {
				charMappingMap_.insert( std::make_pair(chFrom, chTo) );
			}
		}
	}
}

/// -----------------------------------------------------------------------
void Encode::performConvertion( int codePage, const string_t& asciiStr, 
                           string_t& utf8Str )
{
	if ( asciiStr.size() == 0 )
		return;

	wstring_t unicodeStr;
	convertFormat(asciiStr, unicodeStr, codePage);
	convertFormat(unicodeStr, utf8Str, CP_UTF8);
}

/// -----------------------------------------------------------------------
void Encode::performConvertion( const wstring_t& mappingFile, 
                           const string_t& asciiStr,
                           string_t& utf8Str )
{
	/// mappingFile hold file name containing symbol mapping
	loadCharactersMapping(mappingFile);

	wstring_t uniStr;
	uniStr.resize(asciiStr.size());
	for (size_t i = 0; i < asciiStr.size(); ++i) {
		unsigned char wc = asciiStr[i];
		if ( charMapping_[wc] != 0 )
			uniStr[i] = charMapping_[wc];
		else {
			if ( isdigit(wc) || ispunct(wc) || isspace(wc) || wc == 10 || wc == 13 )
				uniStr[i] = wc;
			else {
				uniStr[i] = wc;
				std::cout << "check character " << (int)wc 
					<< " - " << wc << std::endl;
			}
		}
	}

	convertFormat(uniStr, utf8Str, CP_UTF8);
	//writeFileAsBinary()
}
