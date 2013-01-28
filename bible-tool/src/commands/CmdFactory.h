#ifndef CmdFactory_h__
#define CmdFactory_h__

#include "AllCommands.h"

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