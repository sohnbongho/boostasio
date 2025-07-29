#pragma once
#include <memory>
#include "../Zone/ZoneController.h"
#include "../../Model/IInternalMessage.h"
#include "./Component/ZoneManagerComponent.h"

class ZoneControllerManager
{
public:
	static ZoneControllerManager& Instance();
	~ZoneControllerManager();

	void Load();	
	void Tick(int shardId, std::function<void(std::shared_ptr<ZoneController>)> fn);
	void EnqueueMessage(std::shared_ptr<IInternalMessage> msg);

private:
	ZoneControllerManager();

	void InitEcs();	
	void OnRecvHandleMessage(std::shared_ptr<IInternalMessage> message);	

private:
	std::shared_ptr<MessageQueueProcessor> _messageQueueProcessor;
	std::shared_ptr<Entity> _ecsEntity;
};

