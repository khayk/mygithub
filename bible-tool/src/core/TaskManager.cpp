#include "StdAfx.h"
#include "TaskManager.h"
#include "../commands/CmdFactory.h"
#include "../commands/Cmd.h"

/// -----------------------------------------------------------------------
TaskManager::TaskManager()
{
	cmds_.push_back( CmdFactory::createCommand(CMD_ENCODE));
	cmds_.push_back( CmdFactory::createCommand(CMD_COMBINE));
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
	size_t i = 0;
	for ( i = 0; i < cmds_.size(); ++i ) {
		tCmdSp& spCmd = cmds_[i];
		if ( spCmd && cmd == spCmd->getCmdName() ) {

			/// measure time
			namespace pt = boost::posix_time; 
			pt::ptime now1 = pt::microsec_clock::universal_time(); 
			
			/// actual task execution
			spCmd->initialize(cmdArgs);
			spCmd->doCommand();

			pt::ptime now2 = pt::microsec_clock::universal_time(); 
			pt::time_duration dur = now2 - now1; 

			std::wcout << L"command " << spCmd->getCmdName() 
				<< L" completed in " << dur.total_milliseconds()
				<< L" mls" << std::endl;
			break;
		}
	}

	if ( i == cmds_.size() )
		std::wcout << L"Command \"" << cmd << L"\" is not processed" << std::endl;
}

/// -----------------------------------------------------------------------
void TaskManager::displayUsage()
{
	std::cout << "valid commands are:" << std::endl;
	for (size_t i = 0; i < cmds_.size(); ++i) {
		if ( cmds_[i] )
			std::wcout << TEXT("\t") << cmds_[i]->getCmdName() << std::endl;
	}
}


