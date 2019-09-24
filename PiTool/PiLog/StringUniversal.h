#pragma once
#include <string>
#include <windows.h>

#define MAX_DIRECTORY_LEN 255
#define FORMAT_BLOCK_SIZE 512

#ifdef UNICODE
typedef wchar_t  char_str;
#else
typedef char     char_str;
#endif // !UNICODE

class StringUniversal
{
public:
	StringUniversal() {}
	~StringUniversal() {}

	static char* getIp();
	static std::string Format(const char *fmt, ...);
	static std::string FormatV(const char *fmt, va_list args);
	static char* charToutf8(char* cH);
	static char* wharToutf8(WCHAR* wC);
	static WCHAR* utf8Towchar(char* utf);
	static char* wcharTochar(WCHAR* wC);
	static WCHAR* charTowchar(char* cc);
	static void Wchar_tToString(std::string& szDst, wchar_t *wchar);
	static void StringToWstring(std::wstring& szDst, std::string str);
	static std::string to_string(const std::wstring str);
	static std::wstring to_wstring(const std::string str);
	static std::string GetCurrentPath();
	static std::wstring GetCurrentPathW();
	static void GetAppDataPath(std::string& dataPath);
	static void GetAppDataPath(std::wstring& dataPath);
	static void GetLocalAppDataPath(std::string& dataPath);
	static void GetLocalAppDataPath(std::wstring& dataPath);
	//static void GetAppInstallPath(std::string& installPath);
	static bool OnGetPiplayInstall(std::string& szPiplay);
	static bool OnGetPiplayInstall(TCHAR *szPiplay,int size);
	static bool CheckExitsAndCreatePath(std::string& dataPath);
	static int Replace(std::string &str, char oldChar, char newChar);
	static int Replace(std::string &str, const std::string &oldStr, const std::string &newStr);
	static int Replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr);
	static std::string InterceptionString(std::string sourceStr, std::string head, std::string tail, bool headContain = false, bool tailContain = false);
};
