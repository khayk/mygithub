#ifndef __cmdBase_h__
#define __cmdBase_h__

/// -----------------------------------------------------------------------
/// commands base class
class Cmd {
public:
	Cmd(const wstring_t& cmdName);
	virtual ~Cmd();

	wstring_t getCmdName() const;
	wstring_t getArgValue(const wstring_t& argName) const;

	/// interfaces to be implemented by each concrete class
	virtual void initialize(wstring_t& cmdArgs) = 0;
	virtual void doCommand() = 0;
	virtual void showHelp() = 0;

protected:
	bool extractArgumets( wstring_t& cmdArgs, const tWStrVec& argNames);

	wstring_t cmd_;
	typedef std::map<wstring_t, wstring_t> tMapSS;
	tMapSS args_;
};

#endif