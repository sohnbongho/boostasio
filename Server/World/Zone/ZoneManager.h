#pragma once
#include <memory>
#include "ZoneController.h"
#include "../../Model/IInternalMessage.h"

class ZoneManager
{
public:
	static ZoneManager& Instance();

	~ZoneManager();

	void Load();
	void Add(int zoneId, std::shared_ptr<ZoneController> zone);

	void Tick(int shardId, std::function<void(std::shared_ptr<ZoneController>)> fn);
	int GenerateId() { return ++_zoneId; }

	static const int ShardCount = 4;

	std::shared_ptr<ZoneController> GetZone(int zoneId);
	void EnqueueMessage(std::shared_ptr<IInternalMessage> msg);


private:
	struct Shard {
		std::mutex mutex;
		std::unordered_map<uint64_t, std::shared_ptr<ZoneController>> sessions;
	};

	ZoneManager();
	Shard& GetShard(int sessionId);
	void OnRecvHandleMessage(std::shared_ptr<IInternalMessage> message);

private:
	std::array<Shard, ShardCount>  _shards;
	std::atomic<int> _zoneId;
	std::shared_ptr<MessageQueueProcessor> _messageQueueProcessor;
};

