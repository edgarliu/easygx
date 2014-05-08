
#include "sfpr_global.h"
#include "config.h"

#ifdef WIN32
/*
BOOL WINAPI DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
  }
	return TRUE;
}
*/
#endif


char *sfpr_version()
{
	return VERSION;
}
