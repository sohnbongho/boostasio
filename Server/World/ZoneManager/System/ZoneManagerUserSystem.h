#pragma once

#include <memory>
#include "../../../ECS/Entity.h"
#include "../../../ECS/ISystem.h"
#include "../../../Session/User/UserSession.h"

class ZoneManagerUserSystem : ISystem
{
public:
	void AddUser(std::shared_ptr<Entity> ecsEntity, int zoneId, std::shared_ptr<UserSession> user);
};

