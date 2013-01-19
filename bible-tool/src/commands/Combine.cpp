#include "StdAfx.h"
#include "Combine.h"

#define CMB_SRC_DIR      L"--src-dir"
#define CMB_DST_FILE     L"--dst-file"
#define CMB_NAME_MAP     L"--name-mapping"


Combine::Combine( const wstring_t& cmdName )
	: Cmd(cmdName)
{
	//temp();

}

void Combine::initialize( wstring_t& cmdArgs )
{
	tWStrVec argNames;
	argNames.push_back(CMB_SRC_DIR);
	argNames.push_back(CMB_DST_FILE);
	argNames.push_back(CMB_NAME_MAP);

	if ( !extractArgumets(cmdArgs, argNames) )
		throw std::exception("command will not be executed");

	/// destination directory
	wstring_t appDir = getAppDir();
	wstring_t nameMap = getArgValue(CMB_NAME_MAP);
	if (!isAbsolutePath(nameMap))
		nameMap = appDir + L"\\" + nameMap;

	std::wifstream inFile(nameMap, std::ios::in);
	if ( !inFile ) {
		std::wcout << L"failed to open input file: " << nameMap << std::endl;
		return;
	}

	while (!inFile.eof()) {
		wstring_t name, mapping;
		inFile >> name >> mapping;
		if ( !name.empty() && !mapping.empty())
			nameNapping_.insert( std::make_pair(name, mapping) );
	}
}

void Combine::doCommand()
{
	wstring_t appDir = getAppDir();

	/// source directory
	wstring_t srcDir = getArgValue(CMB_SRC_DIR);
	if (!isAbsolutePath(srcDir))
		srcDir = appDir + L"\\" + srcDir;

	/// destination directory
	wstring_t cmbFile = getArgValue(CMB_DST_FILE);
	if (!isAbsolutePath(cmbFile))
		cmbFile = appDir + L"\\" + cmbFile;

	wstring_t cmbResult;

	/// start source directory scanning process
	namespace fs = boost::filesystem;
	fs::path someDir(srcDir);
	fs::directory_iterator end_iter;
	if ( fs::exists(someDir) && fs::is_directory(someDir)) {
		for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter) {
			if (fs::is_regular_file(dir_iter->status()) ) {
				fs::path curFile = *dir_iter;

				auto mapIt = nameNapping_.find(curFile.filename().wstring());
				if ( mapIt == nameNapping_.end() ) {
					/// skip files which are not presented in the mapping
					continue;
				} 

				/// open input file
				std::ifstream srcFile(curFile.wstring(), std::ios::in | std::ios::binary);
				if ( !srcFile ) {
					std::wcout << L"failed to open input file: " << curFile.wstring() << std::endl;
					continue;
				}

				srcFile.seekg(0, std::ios_base::end);
				size_t size = static_cast<size_t>(srcFile.tellg());
				srcFile.seekg(0, std::ios_base::beg);

				string_t utf8Buff;
				if ( size > 0 ) {
					utf8Buff.resize(size);
					srcFile.read(&utf8Buff[0], size);
				}
				else {
					std::wcout << L"zero length file: " << curFile.wstring() << std::endl;
					continue;
				}

				wstring_t uniStr;
				convertFormat(utf8Buff, uniStr, CP_UTF8);

				wstring_t tmp;
				analyzeBook(uniStr, mapIt->second, tmp);
				cmbResult += tmp;

				srcFile.close();
			}
		}
	}

	string_t utf8Combined;
	convertFormat(cmbResult, utf8Combined, CP_UTF8);

	std::ofstream outFile(cmbFile, std::ios::out | std::ios::binary);
	outFile.write(utf8Combined.c_str(), utf8Combined.size());

	outFile.close();
}

void Combine::showHelp()
{
	std::wcout << L"usage: "
		<< cmd_ << L" " << CMB_SRC_DIR << L"=1251 " 
		<< CMB_SRC_DIR << L"=abs or relative directory "
		<< CMB_DST_FILE << L"=resulting file name"
		<< CMB_NAME_MAP << L"=mapping file name"
		<< std::endl;
}

void Combine::analyzeBook( const wstring_t& uniStr, const wstring_t& bookName, wstring_t& res )
{
	std::wstringstream wss(uniStr);

	wstring_t chapStr;
	while (!wss.eof()) {
		wstring_t line;
		getline(wss, line);
		
		if ( line.empty() || (line.size() == 1 && line[0] == 13) || line.size() < 3 )
			continue;

		/// read chapter number
		if ( line.find(L"===") != wstring_t::npos ) {
			size_t pos = line.find(L"===", 4);
			chapStr = line.substr(4, line.find(L"===", 4) - 5);
			chapStr = bookName + L" " + chapStr + L":";
		}
		else if (line.find(L"==") != wstring_t::npos) {
			continue;
		}
		else {
 			res += chapStr;
 			res += line;
// 			if ( isdigit(line[0]) ) {
// 				res += line.substr(0, line.find(' '));
// 				res += 13;
// 				res += 10;
// 				//res += line;
// 			}
// 			else {
// 				res += line;
// 			}
		}
	}
}

void Combine::temp()
{
	wstring_t appDir = getAppDir();
	std::ifstream srcFile(appDir + L"\\res\\kjv invest.txt", std::ios::in | std::ios::binary);
	if ( !srcFile ) {
		return;
	}

	srcFile.seekg(0, std::ios_base::end);
	size_t size = static_cast<size_t>(srcFile.tellg());
	srcFile.seekg(0, std::ios_base::beg);

	string_t utf8Buff;
	if ( size > 0 ) {
		utf8Buff.resize(size);
		srcFile.read(&utf8Buff[0], size);
	}

	wstring_t uniStr;
	convertFormat(utf8Buff, uniStr, CP_UTF8);

	wstring_t res;
	std::wstringstream wss(uniStr);
	while (!wss.eof()) {
		wstring_t line;
		getline(wss, line);

		wstring_t::size_type p = line.find(':');
		p = line.find(' ', p);

		res += line.substr(0, p);
		res += 13;
		res += 10;
	}


	string_t utf8Combined;
	convertFormat(res, utf8Combined, CP_UTF8);

	std::ofstream outFile(appDir + L"\\res\\xkjv.txt", std::ios::out | std::ios::binary);
	outFile.write(utf8Combined.c_str(), utf8Combined.size());

	outFile.close();
}
