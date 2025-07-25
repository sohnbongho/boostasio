#pragma once
#include <unordered_map>
#include <mutex>
#include <memory>
#include "UserSession.h"

class UserSessionManager
{
public:
    static UserSessionManager& Instance()
    {
        static UserSessionManager instance;
        return instance;
    }

    uint64_t AddSession(uint64_t sessionId, std::shared_ptr<UserSession> session);
    void RemoveSession(uint64_t id);
    std::shared_ptr<UserSession> GetSession(uint64_t id);
    void Broadcast(const std::string& message);    
    uint64_t GenerateId();    

private:
    UserSessionManager() = default;
    ~UserSessionManager() = default;    

    std::unordered_map<uint64_t, std::shared_ptr<UserSession>> _sessions;
    std::mutex _mutex;
    std::atomic<uint64_t> _lastId = 0;
};
