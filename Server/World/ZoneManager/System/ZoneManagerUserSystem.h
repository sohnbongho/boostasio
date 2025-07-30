#pragma once

#include <memory>
#include "../../../ECS/Entity.h"
#include "../../../ECS/ISystem.h"
#include "../../../Session/User/UserSession.h"

class ZoneManagerUserSystem : public ISystem
{
public:
	std::shared_ptr<ZoneController> AddUser(std::shared_ptr<Entity> ecsEntity, int zoneId, std::shared_ptr<UserSession> user);
};

