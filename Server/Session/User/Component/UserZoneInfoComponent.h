#pragma once

#include "../../../ECS/Entity.h"
#include "../../../ECS/IComponent.h"
#include "../../../ECS/IDisposable.h"
#include "../../../World/Zone/ZoneController.h"


class UserZoneInfoComponent : public IComponent, public IDisposable
{
public:
	void Dispose() override;

	void SetZone(std::shared_ptr<ZoneController> zone);
	std::weak_ptr<ZoneController>& GetZone();

private:
	std::weak_ptr<ZoneController> _zone;
	
};

