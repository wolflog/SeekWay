//*************************/
/*			Ìú×¦		  */
//*************************/

/************************************************************************/
/*                                                                      */
/*  Auth: SuraWolf														*/
/*  Date: 2016-12-16                                                    */
/*	Desc: Test export function and variable                             */
/*                                                                      */
/************************************************************************/

#include "IronClaw.h"

#ifdef WIN32

#include <Windows.h>

#include "stdio.h"

int num_data;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		num_data = 0;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif
