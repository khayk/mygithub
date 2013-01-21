#include "StdAfx.h"
#include "CmdFactory.h"
#include "Encode.h"
#include "Combine.h"
#include "Analyze.h"

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
    else if (CMD_ANALYZE == cmdString) {
        spCmd.reset(new Analyze(cmdString));
    }

	return spCmd;
}
