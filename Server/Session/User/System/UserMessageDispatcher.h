#pragma once

#include <memory>
#include "../../../ECS/Entity.h"
#include "../../../ECS/ISystem.h"
#include "../../../Model/IInternalMessage.h"

class UserMessageDispatcher : public ISystem
{
public:
	void OnRecvHandleMessage(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message);

private:
	void OnRecvEnterRoomRes(std::shared_ptr<Entity> ecsEntity, std::shared_ptr<IInternalMessage> message);
};

