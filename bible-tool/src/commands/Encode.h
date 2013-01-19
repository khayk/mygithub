#pragma once

#include "Cmd.h"

/// command usage example 
/// --encode --src-encoding=1251 --src-dir=res\rus --dst-dir=res\rus_utf8
/// --encode --src-encoding=res\arm_char_mapping.txt --src-dir=res\arm --dst-dir=res\arm_utf8
class Encode : public Cmd {
public:
	Encode(const wstring_t& cmdName);

	virtual void initialize(wstring_t& cmdArgs);
	virtual void doCommand();
	virtual void showHelp();

private:
	void loadCharactersMapping(const wstring_t& fileName);

	/// content of asciiStr will be converted to Unicode according to codePage
	/// then it will be converted into Unicode
	void performConvertion(int codePage, const string_t& asciiStr, string_t& utf8Str);

	/// content of asciiStr will be converted to Unicode according to rule given
	/// by mappingFile, then it will be converted into Unicode
	void performConvertion(const wstring_t& mappingFile, const string_t& asciiStr, string_t& utf8Str);

	unsigned long cpNumber_;
	
	std::map<wchar_t, wchar_t> charMappingMap_;
	std::vector<wchar_t> charMapping_;
};