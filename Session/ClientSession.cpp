#include "stdafx.h"
#include "ClientSession.h"
#include "Network/IOCPManager.h"
#include "Network/OverlappedContextEx.h"

ClientSession::ClientSession()
{
	// 세션 초기화
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	mConnected = false;
}

ClientSession::~ClientSession()
{
	mConnected = false;
}

bool ClientSession::PrepareSession()
{
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;

	if (SOCKET_ERROR == bind(mSocket, (SOCKADDR*)&addr, sizeof(addr)))
	{
		printf_s("[ ERROR ] bind error: %d\n", GetLastError());
		return false;
	}

	GetSingleton(IOCPManager);
	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, pIOCPManager->GetCompletionPort(), (ULONG_PTR)this, 0);
	if (handle != pIOCPManager->GetCompletionPort())
	{
		printf_s("[ ERROR ] CreateIoCompletionPort error: %d\n", GetLastError());
		return false;
	}

	return true;
}

bool ClientSession::ConnectRequest()
{
	if (mConnected)
	{
		printf_s("[ ERROR ] already connected session");
		return false;
	}

	const char* ip = "127.0.0.1";
	ZeroMemory(&mConnectAddr, sizeof(mConnectAddr));
	mConnectAddr.sin_port = htons(11021);
	mConnectAddr.sin_family = AF_INET;
	mConnectAddr.sin_addr.s_addr = inet_addr(ip);

	OverlappedIOContextEx* context = new OverlappedIOContextEx(this, IO_CONNECT);

	GetSingleton(IOCPManager);
	if (FALSE == pIOCPManager->ConnectEx(mSocket, (sockaddr*)&mConnectAddr, sizeof(SOCKADDR_IN), NULL, 0, NULL, (LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf_s("[ ERROR ] ConnectEx Error : %d\n", GetLastError());
			delete context;
			return false;
		}
	}

	return true;
}

void ClientSession::ConnectCompletion()
{
	mConnected = true;

	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
	{
		printf_s("[ ERROR ] SO_UPDATE_CONNECT_CONTEXT failed: %d \n", GetLastError());
		return;
	}

	int opt = 1;
	if (SOCKET_ERROR == setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		printf_s("[ DEBUG ] TCP_NODELAY error: %d\n", GetLastError());
		return;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		printf_s("[ DEBUG ] SO_RCVBUF change error: %d\n", GetLastError());
		return;
	}

	if (false == PreRecv())
	{
		printf_s("[ DEBUG ] PreRecv for Server Connection error: %d\n", GetLastError());
		return;
	}


	printf_s("[ DEBUG ] Session established: IP=%s, PORT=%d \n", inet_ntoa(mConnectAddr.sin_addr), ntohs(mConnectAddr.sin_port));

}


bool ClientSession::Send(short packetType)
{
	if (!mConnected)
		return false;

	//todo. 패킷 전송
	return true;
}


bool ClientSession::PreRecv()
{
	if (!mConnected)
		return false;

	OverlappedIOContextEx* conetxt = new OverlappedIOContextEx(this, IO_RECV_ZERO);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	conetxt->mWsaBuf.len = 0;
	conetxt->mWsaBuf.buf = nullptr;

	// start async recv
	if (SOCKET_ERROR == WSARecv(mSocket, &conetxt->mWsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)conetxt, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete conetxt;
			printf_s("[ ERROR ] PreRecv Error : %d\n", GetLastError());
			return false;
		}
	}

	return true;
}


bool ClientSession::PostRecv()
{
	if (!mConnected)
		return false;

	OverlappedIOContextEx* context = new OverlappedIOContextEx(this, IO_RECV);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	context->mWsaBuf.len = 0;


	// start real recv
	if (SOCKET_ERROR == WSARecv(mSocket, &context->mWsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)context, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete context;
			printf_s("[ ERROR ] PostRecv Error : %d\n", GetLastError());
			return false;
		}

	}

	return true;
}


