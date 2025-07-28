#include "ZoneUserManager.h"

void ZoneUserManager::Enter(std::shared_ptr<UserSession> user)
{
	_users[user->GetUid()] = user;
}

void ZoneUserManager::RemoveUser(uint64_t userId)
{
	_users.erase(userId);
}

void ZoneUserManager::Tick()
{
	for (auto& user : _users)
	{
		auto& session = user.second;
		session->Tick();
	}
}

void ZoneUserManager::Broadcast(const std::string& msg)
{
	
}
