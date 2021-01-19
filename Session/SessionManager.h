#pragma once
#include "ClientSession.h"

using namespace std;

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

public:
	bool Init();
	bool Start();

private:
	typedef	vector<ClientSession*> ClientSessions;

	ClientSessions mClientSessions;
	mutex mLock;
};