#pragma once
#include "../../../ECS/ISystem.h"
#include "../../../ECS/Entity.h"
#include <memory>
#include "../../../Model/IInternalMessage.h"

class ZoneManagerMessageDispatcher : public ISystem
{
public:
	void OnRecvHandleMessage(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message);

	void OnRecvEnterRoomReq(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message);
};

