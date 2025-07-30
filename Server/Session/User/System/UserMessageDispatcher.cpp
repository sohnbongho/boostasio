#include "UserMessageDispatcher.h"
#include "../Component/UserZoneInfoComponent.h"

void UserMessageDispatcher::OnRecvHandleMessage(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message)
{
	auto messageType = message->GetMessageType();
	std::cout << "OnRecvHandleMessage messageType:" << messageType << std::endl;

	switch (messageType)
	{
	case InternalMessageType::EnterRoomRes:
		OnRecvEnterRoomRes(ecsEntity, message);
		break;
	default:
		break;
	}
}
void UserMessageDispatcher::OnRecvEnterRoomRes(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message)
{
	auto zoneComponent = ecsEntity->GetComponent< UserZoneInfoComponent>();
	if (zoneComponent == nullptr)
		return;
	auto request = std::static_pointer_cast<EnterRoomResponse>(message);
	
	auto zone = request->ZoneController;
	zoneComponent->SetZone(zone);

}