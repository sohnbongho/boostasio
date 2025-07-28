#include "UserSessionShardManager.h"
#include <iostream>

UserSessionShardManager& UserSessionShardManager::Instance()
{
    static UserSessionShardManager instance;
    return instance;
}

UserSessionShardManager::UserSessionShardManager()
{    
}

UserSessionShardManager::Shard& UserSessionShardManager::GetShard(uint64_t sessionId)
{
    return _shards[sessionId % SHARD_COUNT];
}

void UserSessionShardManager::AddSession(uint64_t sessionId, std::shared_ptr<UserSession> session)
{
    auto& shard = GetShard(sessionId);
    std::lock_guard<std::mutex> lock(shard.mutex);
    shard.sessions[sessionId] = session;
}

void UserSessionShardManager::RemoveSession(uint64_t sessionId)
{
    auto& shard = GetShard(sessionId);
    std::lock_guard<std::mutex> lock(shard.mutex);
    shard.sessions.erase(sessionId);
}

std::shared_ptr<UserSession> UserSessionShardManager::GetSession(uint64_t sessionId)
{
    auto& shard = GetShard(sessionId);
    std::lock_guard<std::mutex> lock(shard.mutex);
    auto it = shard.sessions.find(sessionId);
    if (it != shard.sessions.end())
        return it->second;
    return nullptr;
}

void UserSessionShardManager::TickAll()
{
    for (auto& shard : _shards)
    {
        std::lock_guard<std::mutex> lock(shard.mutex);
        for (auto& session : shard.sessions)
        {
            auto& user = session.second;
            user->Tick(); // UserSession 안에 Tick() 메소드 필요
        }
    }
}

void UserSessionShardManager::TickShard(int shardId)
{
    if (shardId < 0 || shardId >= SHARD_COUNT)
        return;

    auto& shard = _shards[shardId];
    std::lock_guard<std::mutex> lock(shard.mutex);
    for (auto& session : shard.sessions)
    {
        auto& user = session.second;
        user->Tick(); // UserSession 안에 Tick() 메소드 필요
    }
}
