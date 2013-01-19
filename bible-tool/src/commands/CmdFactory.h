#ifndef __cmdFactory_h__
#define __cmdFactory_h__

#define CMD_ENCODE  L"--encode"
#define CMD_COMBINE L"--combine"

/// -----------------------------------------------------------------------
/// command factory
class CmdFactory {
public:
	CmdFactory();
	virtual ~CmdFactory();

	/// create command
	static tCmdSp createCommand( const wstring_t& cmdString );
};

#endif