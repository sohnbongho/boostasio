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

    uint64_t AddSession(uint64_t sessionId, std::shared_ptr<UserSession> session)
    {
        std::lock_guard<std::mutex> lock(_mutex);        
        _sessions[sessionId] = session;
        return sessionId;
    }

    void RemoveSession(uint64_t id)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.erase(id);
    }

    std::shared_ptr<UserSession> GetSession(uint64_t id)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _sessions.find(id);
        return (it != _sessions.end()) ? it->second : nullptr;
    }

    void Broadcast(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (auto& kv : _sessions)
        {
            auto& session = kv.second;
            if (session)
                session->Send(message);
        }
    }
    uint64_t GenerateId()
    {
        return ++_lastId;
    }

private:
    UserSessionManager() = default;
    ~UserSessionManager() = default;

    

    std::unordered_map<uint64_t, std::shared_ptr<UserSession>> _sessions;
    std::mutex _mutex;
    std::atomic<uint64_t> _lastId = 0;
};
