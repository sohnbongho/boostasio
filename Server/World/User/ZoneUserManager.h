#pragma once
#include <memory>
#include <string>
#include "../../Session/User/UserSession.h"

class ZoneUserManager
{
public:
    ZoneUserManager();
    virtual ~ZoneUserManager();
    void Enter(std::shared_ptr<UserSession> user);
    void RemoveUser(uint64_t userId);
    void Tick();
    void Broadcast(const std::string& msg);

private:
    std::unordered_map<uint64_t, std::shared_ptr<UserSession>> _users;
    std::shared_ptr<MessageQueueProcessor> _messageQueueProcessor;
};


