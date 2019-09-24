#pragma once
#include  <shlobj.h>
#include <windows.h>
#include <iostream>

#pragma warning( push ) 
#pragma warning( disable : 4091 )
#include <DbgHelp.h>
#pragma warning( pop )

class Exception {
public:
	Exception();
	~Exception();
	static bool WriteMinidump(EXCEPTION_POINTERS* pEp, std::string name = "crash");
	static bool WriteStacktrace(EXCEPTION_POINTERS* pEp, std::string name = "crash");

private:
	static std::string mVersion;

};