#include "stdafx.h"
#include "IOWorkerThread.h"
#include "Network/OverlappedContextEx.h"

IOWorkerThread::IOWorkerThread(HANDLE hCompletionPort)
	: mCompletionPort(hCompletionPort)
	, mIsWorkerRun(true)
{
}
IOWorkerThread::~IOWorkerThread()
{
}

void IOWorkerThread::IOWorker()
{

	printf_s("[ INFO ] IO worker thread Start..\n");

	DWORD dwTransferred = 0;
	OverlappedIOContextEx* context = nullptr;
	ULONG_PTR completionKey = 0;

	while (mIsWorkerRun)
	{
		int ret = GetQueuedCompletionStatus(mCompletionPort, &dwTransferred, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&context, 10);

		ClientSession* session = context ? context->mSessionObject : nullptr;

		if (ret == 0 || dwTransferred == 0)
		{
			if (completionKey == 0xDEAD)
			{
				mIsWorkerRun = false;
				break;
			}
			
			if (context == nullptr && GetLastError() == WAIT_TIMEOUT)
				continue;

			if (context->mIoType == IO_RECV || context->mIoType == IO_SEND)
			{
				continue;
			}
		}


		switch (context->mIoType)
		{
		case IO_CONNECT:
			session->ConnectCompletion();
			printf_s("[ DEBUG ] connect context recieved.\n");
			break;

		case IO_DISCONNECT:

			printf_s("[ DEBUG ] disconnect context recieved.\n");
			break;

		case IO_RECV_ZERO:
			session->PostRecv();
			printf_s("[ DEBUG ] recvZero context recieved.\n");
			break;

		case IO_SEND:
			printf_s("[ DEBUG ] send context recieved.\n");
			break;

		case IO_RECV:

			printf_s("[ DEBUG ] recv context recieved.\n");

			break;

		default:
			printf_s("[ DEBUG ] Unknown I/O Type: %d\n", context->mIoType);

			break;
		}

	}

	// todo. ½ÇÆÐ ½Ã Disconnect
}
