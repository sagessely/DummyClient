#pragma once

class ClientSession
{
public:
	ClientSession();
	~ClientSession();


	bool PreRecv();
	bool PostRecv();

	bool PrepareSession();

	bool ConnectRequest();
	void ConnectCompletion();

	bool Send(short packetType);

	void UpdateConnetedFlag(bool flag) {
		mConnected = flag;
	};

private:


	bool mConnected;
	SOCKET mSocket;
	SOCKADDR_IN mConnectAddr;
};



