#pragma once
#include "stdafx.h"
#include "Session/ClientSession.h"
enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_RECV,
	IO_RECV_ZERO,

	IO_DISCONNECT,
	IO_CONNECT
};

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
	DR_SENDFLUSH_ERROR
};


struct OverlappedIOContextEx
{
	OverlappedIOContextEx(ClientSession* owner, IOType ioType)
		: mSessionObject(owner)
		, mIoType(ioType)
	{
		memset(&mOverlapped, 0, sizeof(OVERLAPPED));
		memset(&mWsaBuf, 0, sizeof(WSABUF));
		//mSessionObject->AddRef();
	}

	OVERLAPPED	mOverlapped;
	ClientSession* mSessionObject;
	IOType		mIoType;
	WSABUF		mWsaBuf;

};
