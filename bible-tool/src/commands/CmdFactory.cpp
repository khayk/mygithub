#include "StdAfx.h"
#include "CmdFactory.h"
#include "Encode.h"
#include "Combine.h"

/// -----------------------------------------------------------------------
tCmdSp CmdFactory::createCommand( const wstring_t& cmdString )
{
	tCmdSp spCmd;
	if (CMD_ENCODE == cmdString) {
		spCmd.reset(new Encode(cmdString));
	}
	else if (CMD_COMBINE == cmdString) {
		spCmd.reset(new Combine(cmdString));
	}

	return spCmd;
}
