#pragma once

#include "Thread/IOWorkerThread.h"

class IOCPManager
{
public:
	IOCPManager();
	~IOCPManager();

	bool Init();
	void Clear();


	bool Start();
	bool Stop();


	BOOL ConnectEx(SOCKET hSocket, const struct sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength,
		LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
	{
		return mFnConnectEx(hSocket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped);
	};

	HANDLE GetCompletionPort() { return mCompletionPort; }

	LPFN_CONNECTEX mFnConnectEx;
	LPFN_DISCONNECTEX mFnDisconnectEx;
	
private:


	//IO Worker ½º·¹µå
	std::vector<std::thread> mIOWorkerThreads;

	std::mutex mLock;
	HANDLE mCompletionPort;
	int mThreadCount;

};
