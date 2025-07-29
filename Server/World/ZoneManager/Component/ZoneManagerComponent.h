#pragma once

#include <memory>
#include "../../../ECS/IComponent.h"
#include "../../../ECS/IDisposable.h"
#include "../../../ECS/Entity.h"
#include "../../Zone/ZoneController.h"
#include "../../../Const/ConstInfo.h"

class ZoneManagerComponent : public IComponent, public IDisposable
{
public:
	struct Shard {
		std::mutex mutex;
		std::unordered_map<uint64_t, std::shared_ptr<ZoneController>> sessions;
	};

	ZoneManagerComponent();
	virtual ~ZoneManagerComponent();
	void Dispose() override;

	int GenerateId();	
	Shard& GetShardsByZoneId(int zoneId);
	Shard& GetShardsByShardId(int shardId);	

	void AddZone(int zoneId, std::shared_ptr<ZoneController> zone);
	void AddUser(int zoneId, std::shared_ptr<UserSession> user);

private:	

	static const int ShardCount = ConstInfo::ShardCount;

	std::array<Shard, ShardCount>  _shards;
	std::atomic<int> _zoneId;
};

