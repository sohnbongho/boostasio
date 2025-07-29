#include "UserZoneInfoComponent.h"

void UserZoneInfoComponent::Dispose() 
{

}

void UserZoneInfoComponent::SetZone(std::shared_ptr<ZoneController> zone) 
{
	_zone = std::weak_ptr<ZoneController>(zone);
}

std::weak_ptr<ZoneController>& UserZoneInfoComponent::GetZone() 
{
	return _zone;
}