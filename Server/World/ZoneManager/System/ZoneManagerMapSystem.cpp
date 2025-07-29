#include "ZoneManagerMapSystem.h"
#include "../Component/ZoneManagerComponent.h"


void ZoneManagerMapSystem::LoadMaps(std::shared_ptr<Entity> ecsEntity)
{
	auto manager = ecsEntity->GetComponent<ZoneManagerComponent>();
	if (manager == nullptr)
		return;

	{
		int mapId = 1001;
		int zoneId = manager->GenerateId();
		auto zone = std::make_shared<ZoneController>(zoneId, mapId);
		zone->Load();
		Add(ecsEntity, zoneId, zone);
	}
	{
		int mapId = 1002;
		int zoneId = manager->GenerateId();
		auto zone = std::make_shared<ZoneController>(zoneId, mapId);
		zone->Load();
		Add(ecsEntity, zoneId, zone);
	}
	{
		int mapId = 1003;
		int zoneId = manager->GenerateId();
		auto zone = std::make_shared<ZoneController>(zoneId, mapId);
		zone->Load();
		Add(ecsEntity, zoneId, zone);
	}
}

void ZoneManagerMapSystem::Add(std::shared_ptr<Entity> ecsEntity, int zoneId, std::shared_ptr<ZoneController> zone)
{
	auto component = ecsEntity->GetComponent<ZoneManagerComponent>();
	if (component == nullptr)
		return;

	component->AddZone(zoneId, zone);	
}

