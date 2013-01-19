// BibleTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TaskManager.h"


/// -----------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	try {
		TaskManager taskMan;
		taskMan.doTask(argc, argv);
	}
	catch (const std::exception& e) {
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception" << std::endl;
	}

	return 0;
}

