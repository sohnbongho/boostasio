#pragma once
#include "../../../ECS/ISystem.h"
#include <memory>
#include "../../../ECS/Entity.h"
#include "../../Zone/ZoneController.h"

class ZoneManagerMapSystem : public ISystem
{
public:
	void LoadMaps(std::shared_ptr<Entity> ecsEntity);
	void Add(std::shared_ptr<Entity> ecsEntity, int zoneId, std::shared_ptr<ZoneController> zone);	
};
