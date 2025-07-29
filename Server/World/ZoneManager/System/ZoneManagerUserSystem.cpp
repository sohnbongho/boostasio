#include "ZoneManagerUserSystem.h"
#include "../Component/ZoneManagerComponent.h"


void ZoneManagerUserSystem::AddUser(std::shared_ptr<Entity> ecsEntity, int zoneId, std::shared_ptr<UserSession> user)
{
	auto zoneComponent = ecsEntity->GetComponent<ZoneManagerComponent>();
	if (zoneComponent == nullptr)
		return;

	auto& shard = zoneComponent->GetShardsByZoneId(zoneId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	auto& zone = shard.sessions[zoneId];
	zone->AddUser(user);
}
