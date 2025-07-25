#include "UserSessionManager.h"

uint64_t UserSessionManager::AddSession(uint64_t sessionId, std::shared_ptr<UserSession> session)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions[sessionId] = session;
    return sessionId;
}

void UserSessionManager::RemoveSession(uint64_t id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.erase(id);
}

std::shared_ptr<UserSession> UserSessionManager::GetSession(uint64_t id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _sessions.find(id);
    return (it != _sessions.end()) ? it->second : nullptr;
}

void UserSessionManager::Broadcast(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);
    for (auto& kv : _sessions)
    {
        auto& session = kv.second;
        if (session)
            session->Send(message);
    }
}
uint64_t UserSessionManager::GenerateId()
{
    return ++_lastId;
}
