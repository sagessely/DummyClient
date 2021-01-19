#include "stdafx.h"
#include "SessionManager.h"
const int MAX_CONNECTION = 3;

SessionManager::SessionManager()
{

}
SessionManager::~SessionManager()
{

}

bool SessionManager::Init()
{
	for (int i = 0; i < MAX_CONNECTION; ++i)
	{
		ClientSession* client = new ClientSession();

		if (false == client->PrepareSession())
			return false;

		mClientSessions.push_back(client);
	}
	return true;
}


bool SessionManager::Start()
{
	
	for (auto session : mClientSessions)
	{
		if (false == session->ConnectRequest())
			return false;

		Sleep(10);
	}
	
	return true;
}