#pragma once
#include <atlstr.h>
#include <tlhelp32.h>
#include "windows.h"
#include <string>

class WinCommon
{
private:
	WinCommon() {}
	~WinCommon() {}
public:
	static std::pair<bool, unsigned long> startProcess(std::wstring filename, bool wait, std::wstring dir_path = NULL, bool showWin = true, std::wstring param = L"");
	static std::pair<bool, unsigned long> startProcess(std::wstring sPath, std::wstring cmd, std::wstring sEnv, std::wstring sCurrentPath);
	static bool RunAsAdmin(std::wstring path);
	static bool isServiceAlive(WCHAR* name);
	static bool processAlived(std::string name);
	static bool processAlived(std::wstring name);
	static void killProcessByName(WCHAR* szName);
	static void killProcessByName(std::string name);
	static bool fileExists(std::wstring szPath);
	static bool fileExists(std::string szPath);
	static bool dirExists(std::wstring szPath);
	static bool dirExists(std::string szPath);
	static bool createLinkFile(std::wstring szStartAppPath, std::wstring szAddCmdLine, std::wstring szDestLnkPath, std::wstring szIconPath);
};
