#include "ZoneUserComponent.h"

ZoneUserComponent::ZoneUserComponent()
{

}

ZoneUserComponent::~ZoneUserComponent() 
{
	Dispose();
}


void ZoneUserComponent::Dispose()
{
    _users.clear();
}

void ZoneUserComponent::Add(std::shared_ptr<UserSession> user)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_users[user->GetUid()] = user;
}

void ZoneUserComponent::Remove(uint64_t userUid)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_users.erase(userUid);
}

void ZoneUserComponent::Tick()
{
    std::lock_guard<std::mutex> lock(_mutex);
    for (auto it = _users.begin(); it != _users.end(); )
    {
        auto user = it->second.lock(); 
        if (user)
        {
            user->Tick();
            ++it;
        }
        else
        {
            it = _users.erase(it);  // expired된 유저 제거
        }
    }
}