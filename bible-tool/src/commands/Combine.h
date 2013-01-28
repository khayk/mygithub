#ifndef Combine_h__
#define Combine_h__

#include "Cmd.h"

/// command usage example 
/// --combine --src-dir=res\rus_utf8 --dst-file=res\synodal.txt --name-mapping=name_mapping.txt
/// 
class Combine : public Cmd {
public:
	Combine(const wstring_t& cmdName);

	virtual void initialize(wstring_t& cmdArgs);
	virtual void doCommand();
	virtual void showHelp();

private:
	void analyzeBook(const wstring_t& uniStr, const wstring_t& bookName, wstring_t& res);

	void temp();
	std::map<wstring_t, wstring_t> nameNapping_;
};

#endif // Combine_h__