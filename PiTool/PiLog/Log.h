#pragma once
#include <string>
#include <mutex>
#include "WinInterfaceForLog.h"

#define LOG_LEVEL_DEBUG      2
#define LOG_LEVEL_NORMAL      1
#define LOG_LEVEL_NONE            0

#define LOGFATAL                      6
#define LOGERROR                     5
#define LOGWARNING               4
#define LOGNOTICE                  3
#define LOGRECORD                 2
#define LOGDEBUG                  1
#define LOGNONE                   0

#define APPNAME      "pimaxService"
#define APPVERSION   "1.7.1"
#define APPLOGHOME    "\\pimax\\"
#define APPLOGINSTALL    "Pilog\\PiService\\"
#define APPLOCAL_LOGINSTALL    "\\Pimax\\PiService\\"


class Log
{
private:
	Log();
	~Log();
public:
	static FILE* file;
	static int logLevel;
	static void PRINTLOG(int logtype, const char *format, ...);
	static void LOG(int loglevel, const char *format, ...);
	static void LOGW(int logtype, const wchar_t* str);
	static bool IsLogLevel(int level);
	static int GetLogLevel();
	static void SetLogLevel(int level);
	static bool Init(const std::string& path);
	static bool InitWithTime(const std::string& path);
	static bool InitLog(const std::string& name);
	static bool InitPiLog(const std::string& path);
	static void Close();
private:
	static void LogString(int logLevel, const std::string& logString);
	static bool WriteLogString(int logLevel, const std::string& logString);
	static Log* logInstance;
	WinInterfaceForLog winLog;
	std::mutex mutex;
};
