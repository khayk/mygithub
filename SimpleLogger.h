#pragma once

#include <fstream>
#include <string>
#include <ctime>

inline void logMsg(const std::string& msg) {
// 	static std::ofstream outFile("appLog.log", std::ios::out);
// 	outFile << msg;

	static bool sFirstRun = true;
	static std::ofstream outF;
	if (sFirstRun) {
		sFirstRun = false;
		std::string name = "D:\\appLog.txt";
		outF.open(name.c_str(), std::ios::out);
	}
	outF << time(NULL) << ": " << msg << std::endl;
}

