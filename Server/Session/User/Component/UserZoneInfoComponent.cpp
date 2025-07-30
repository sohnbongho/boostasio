#include "UserZoneInfoComponent.h"

UserZoneInfoComponent::~UserZoneInfoComponent() 
{
	Dispose();
}

void UserZoneInfoComponent::Dispose() 
{
	_zone = nullptr;
}

void UserZoneInfoComponent::SetZone(std::shared_ptr<ZoneController> zone) 
{
	_zone = std::weak_ptr<ZoneController>(zone);
}

std::weak_ptr<ZoneController>& UserZoneInfoComponent::GetZone() 
{
	return _zone;
}