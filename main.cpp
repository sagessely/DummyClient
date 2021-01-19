#include "stdafx.h"

#include "Session/ClientSession.h"
#include "Network/IOCPManager.h"
#include "Session/SessionManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf_s("[ INFO ] Welcome to DummyClient.\n");


	// 매니징 시스템 초기화 ////////////////////////////////////////////////
	printf_s("[ INFO ] Initializing Manager System.\n");

	// 1. IOCP Manager		////////////////////////////////////////////////
	GetSingleton(IOCPManager);
	if (!pIOCPManager->Init())
	{
		printf_s("[ ERROR ] Fail to Initializing IOCP Manager System.\n");
		return 0;
	}
	if (!pIOCPManager->Start())
	{
		printf_s("[ ERROR ] Fail to Start IOCP Manager System.\n");
		return 0;
	}

	printf_s("[ INFO ] IOCP Manager initialization is complete.\n");

	// 2. Sessoin Manager	////////////////////////////////////////////////
	GetSingleton(SessionManager);
	if (!pSessionManager->Init())
	{
		printf_s("[ ERROR ] Fail to Initializing Session Manager System...\n");
		return 0;
	}
	if (!pSessionManager->Start())
	{
		printf_s("[ ERROR ] Fail to Start Session Manager System...\n");
		return 0;
	}

	printf_s("[ INFO ] Session Manager initialization is complete....\n");


	////////////////////////////////////////////////////////////////////////
	getchar();
	
	printf_s("[ INFO ] Shutdown DummyClient...\n");

	return 0;
}