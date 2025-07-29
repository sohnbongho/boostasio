#include "ZoneManagerMessageDispatcher.h"
#include "../Component/ZoneManagerComponent.h"
#include "../../../Session/Manager/UserSessionShardManager.h"
#include "ZoneManagerUserSystem.h"

void ZoneManagerMessageDispatcher::OnRecvHandleMessage(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message)
{
	auto messageType = message->GetMessageType();
	std::cout << "OnRecvHandleMessage messageType:" << messageType << std::endl;

	switch (messageType)
	{
	case InternalMessageType::EnterRoomReq:
		OnRecvEnterRoomReq(ecsEntity, message);
		break;
	default:
		break;
	}
}

void ZoneManagerMessageDispatcher::OnRecvEnterRoomReq(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message)
{
	auto request = std::static_pointer_cast<EnterRoomReuqest>(message);

	auto zoneComponent = ecsEntity->GetComponent<ZoneManagerComponent>();
	if (zoneComponent == nullptr)
		return;

	auto userSystem = ecsEntity->GetComponent<ZoneManagerUserSystem>();
	if (userSystem == nullptr)
		return;

	auto zoneId = request->ZoneId;
	auto sessionId = request->UserSessionId;

	auto user = UserSessionShardManager::Instance().Get(sessionId);
	if (user)
	{
		userSystem->AddUser(ecsEntity, zoneId, user);

		auto message = std::make_shared<EnterRoomResponse>(zoneId, sessionId);
		user->EnqueueMessage(message);
	}
}
