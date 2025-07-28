#pragma once
#include <memory>
#include "ZoneController.h"

class ZoneManager
{
public:
	static ZoneManager& Instance();

	void Init();
	void Add(int zoneId, std::shared_ptr<ZoneController> zone);

	void Tick(int shardId, std::function<void(std::shared_ptr<ZoneController>)> fn);
	int GenerateId() { return ++_zoneId; }

	static const int ShardCount = 4;

private:
	ZoneManager();
	

	struct Shard {
		std::mutex mutex;
		std::unordered_map<uint64_t, std::shared_ptr<ZoneController>> sessions;
	};

	std::array<Shard, ShardCount>  _shards;
	std::atomic<int> _zoneId;

	int GetShardIndex(int zoneId);
	Shard& GetShard(int sessionId);

	int _zoneThreadCount = 1;



};

