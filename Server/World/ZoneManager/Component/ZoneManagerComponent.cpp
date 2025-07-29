#include "ZoneManagerComponent.h"

ZoneManagerComponent::ZoneManagerComponent() 
	: _zoneId(0)
{
}

ZoneManagerComponent::~ZoneManagerComponent()
{
	
}

void ZoneManagerComponent::Dispose()
{
}

int ZoneManagerComponent::GenerateId() 
{ 
	return ++_zoneId; 
}


ZoneManagerComponent::Shard& ZoneManagerComponent::GetShardsByZoneId(int zoneId)
{
	return _shards[zoneId % ShardCount];
}

ZoneManagerComponent::Shard& ZoneManagerComponent::GetShardsByShardId(int shardId)
{
	return _shards[shardId];
}

void ZoneManagerComponent::AddZone(int zoneId, std::shared_ptr<ZoneController> zone) 
{
	auto& shard = GetShardsByZoneId(zoneId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	shard.sessions[zoneId] = std::move(zone);
}

