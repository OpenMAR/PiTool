#pragma once
#include <string>

typedef void* HANDLE; // forward declaration, to avoid inclusion of whole Windows.h

class WinInterfaceForLog
{
public:
	WinInterfaceForLog();
	~WinInterfaceForLog();
	bool OpenLogFile(const std::string& logFilename);
	bool OpenLogFile(const std::string& logFilename, const std::string& backupOldLogToFilename);
	void CloseLogFile(void);
	bool WriteStringToLog(const std::string& logString);
	void PrintDebugString(const std::string& debugString);
	static void GetCurrentLocalTime(int& hour, int& minute, int& second);
private:
	HANDLE m_hFile;
};

