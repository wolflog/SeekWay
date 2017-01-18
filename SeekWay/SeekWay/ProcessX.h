#pragma once
#include "DeityList.h"
#include "YString.h"

#ifdef WIN32
#include <Windows.h>

#else



#endif 

namespace Process
{
	//获得进程名  
	YString GetProcessName(unsigned long processID){  
		wchar_t procName[MAX_PATH] = {0};  
		YString str_proc_name;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,processID);  
		GetModuleFileNameEx(hProcess,NULL,procName,MAX_PATH);  
		CloseHandle(hProcess); 
		str_proc_name = procName;
		return str_proc_name;  
	}
}