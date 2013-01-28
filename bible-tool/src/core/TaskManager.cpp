#include "StdAfx.h"
#include "TaskManager.h"
#include "../commands/CmdFactory.h"
#include "../commands/Cmd.h"

/// -----------------------------------------------------------------------
TaskManager::TaskManager()
{
}

/// -----------------------------------------------------------------------
TaskManager::~TaskManager()
{

}

/// -----------------------------------------------------------------------
void TaskManager::doTask( int argc, _TCHAR* argv[] )
{
	if ( argc < 2 ) {
		displayUsage();
		return;
	}

	wstring_t cmdArgs;
	for ( int j = 2; j < argc; ++j ) {
		cmdArgs += argv[j];
		cmdArgs += ( (j < argc - 1) ? _T(" ") : _T("") );
	}
	
	/// now let the command read its own arguments and do requested task
	wstring_t cmd = argv[1];
	cmd.erase(0, 2);

    tCmdSp spCmd = CmdFactory::createCommand(cmd);
	if ( spCmd ) {
        DWORD now1 = GetTickCount();
			
		/// actual task execution
		spCmd->initialize(cmdArgs);
		spCmd->doCommand();

        DWORD now2 = GetTickCount();

		std::wcout << L"command " << spCmd->getCmdName() 
			<< L" completed in " << now2 - now1
			<< L" mls" << std::endl;
	}
    else {
        std::wcout << L"Command \"" << cmd << L"\" is not processed" << std::endl;
        displayUsage();
    }
}

/// -----------------------------------------------------------------------
void TaskManager::displayUsage()
{
	std::wcout << L"valid commands are:" << std::endl;
    wstring_t cmds(
        L"\t" CMD_ENCODE L" (perform data encoding)\n"
        L"\t" CMD_COMBINE L" (combine input data into single place)\n"
		L"\t" CMD_ANALYZE L" (analyze source based on selected criteria)\n"
    );

   	std::wcout << cmds << std::endl;
}


