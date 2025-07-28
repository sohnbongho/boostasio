#include "UserSessionShardManager.h"
#include <iostream>

UserSessionShardManager& UserSessionShardManager::Instance()
{
	static UserSessionShardManager instance;
	return instance;
}

UserSessionShardManager::UserSessionShardManager() : _userSessionId(0)
{
}

UserSessionShardManager::Shard& UserSessionShardManager::GetShard(uint64_t sessionId)
{
	return _shards[sessionId % ShardCount];
}

void UserSessionShardManager::Add(uint64_t sessionId, std::shared_ptr<UserSession> session)
{
	auto& shard = GetShard(sessionId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	shard.sessions[sessionId] = session;
}

void UserSessionShardManager::Remove(uint64_t sessionId)
{
	auto& shard = GetShard(sessionId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	shard.sessions.erase(sessionId);
}

std::shared_ptr<UserSession> UserSessionShardManager::Get(uint64_t sessionId)
{
	auto& shard = GetShard(sessionId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	auto it = shard.sessions.find(sessionId);
	if (it != shard.sessions.end())
		return it->second;
	return nullptr;
}
