#include "stdafx.h"
#include "IOCPManager.h"

IOCPManager::IOCPManager()
{
}
IOCPManager::~IOCPManager()
{
}

bool IOCPManager::Init()
{
	/// set num of I/O threads
	mThreadCount = 4;

	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	/// Create I/O Completion Port
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mCompletionPort == NULL)
		return false;

	/// create TCP socket
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
		return false;

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, mCompletionPort, 0, 0);
	if (handle != mCompletionPort)
	{
		printf_s("[DEBUG] listen socket IOCP register error: %d\n", GetLastError());
		return false;
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	GUID guidDisconnectEx = WSAID_DISCONNECTEX;
	DWORD bytes = 0;
	if (SOCKET_ERROR == WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidDisconnectEx, sizeof(GUID), &mFnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL))
		return false;

	GUID guidConnectEx = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(GUID), &mFnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL))
		return false;

	return true;
}

void IOCPManager::Clear()
{
	WSACleanup();
}

bool IOCPManager::Start()
{
	auto threadCount = 10;
	for (UINT32 i = 0; i < threadCount; i++)
	{
		IOWorkerThread* worker = new IOWorkerThread(mCompletionPort);
		mIOWorkerThreads.emplace_back([this, worker]() { worker->IOWorker(); });
	}

	return true;
}

bool IOCPManager::Stop()
{
	return true;
}
