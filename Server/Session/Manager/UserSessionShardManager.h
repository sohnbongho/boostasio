#pragma once

#include <unordered_map>
#include <shared_mutex>
#include <memory>
#include <vector>
#include <mutex>
#include "../User/UserSession.h"
#include "../../Const/ConstInfo.h"

class UserSessionShardManager
{
public:
	static UserSessionShardManager& Instance();

	void Add(uint64_t sessionId, std::shared_ptr<UserSession> session);
	void Remove(uint64_t sessionId);
	std::shared_ptr<UserSession> Get(uint64_t sessionId);		
	
	uint64_t GenerateId() { return _userSessionId++; }

private:
	UserSessionShardManager();
	static const int ShardCount = ConstInfo::ShardCount;

	struct Shard {
		std::mutex mutex;
		std::unordered_map<uint64_t, std::shared_ptr<UserSession>> sessions;		
	};

	std::array<Shard, ShardCount>  _shards;
	std::atomic<uint64_t > _userSessionId;

	Shard& GetShard(uint64_t sessionId);
};
