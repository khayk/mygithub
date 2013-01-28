#ifndef CmdFactory_h__
#define CmdFactory_h__

#define CMD_ENCODE  L"--encode"
#define CMD_COMBINE L"--combine"
#define CMD_ANALYZE L"--analyze"

/// -----------------------------------------------------------------------
/// command factory
class CmdFactory {
public:
	CmdFactory();
	virtual ~CmdFactory();

	/// create command
	static tCmdSp createCommand( const wstring_t& cmdString );
};

#endif // CmdFactory_h__