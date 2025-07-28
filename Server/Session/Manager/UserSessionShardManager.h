#pragma once

#include <unordered_map>
#include <shared_mutex>
#include <memory>
#include <vector>
#include <mutex>
#include "../User/UserSession.h"

class UserSessionShardManager
{
public:
	static UserSessionShardManager& Instance();

	void AddSession(uint64_t sessionId, std::shared_ptr<UserSession> session);
	void RemoveSession(uint64_t sessionId);
	std::shared_ptr<UserSession> GetSession(uint64_t sessionId);
	void TickAll();
	void TickShard(int shardId);
	uint64_t GenerateId() { return _userId++; }

private:
	UserSessionShardManager();
	static const int SHARD_COUNT = 4;

	struct Shard {
		std::mutex mutex;
		std::unordered_map<uint64_t, std::shared_ptr<UserSession>> sessions;		
	};

	std::array<Shard, SHARD_COUNT>  _shards;
	std::atomic<uint64_t > _userId;


	Shard& GetShard(uint64_t sessionId);
};
