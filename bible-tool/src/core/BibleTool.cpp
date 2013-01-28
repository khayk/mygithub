// BibleTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TaskManager.h"

/// -----------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    DWORD now1 = GetTickCount();

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

    DWORD now2 = GetTickCount();
    std::wcout << L"application run time " << now2 - now1
        << L" mls" << std::endl;

	return 0;
}

