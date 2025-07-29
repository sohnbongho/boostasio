#include "ZoneControllerManager.h"
#include "System/ZoneManagerMapSystem.h"
#include "System/ZoneManagerMessageDispatcher.h"
#include "System/ZoneManagerUserSystem.h"

ZoneControllerManager& ZoneControllerManager::Instance()
{
	static ZoneControllerManager instance;
	return instance;
}

ZoneControllerManager::ZoneControllerManager()
	: _messageQueueProcessor(std::make_shared< MessageQueueProcessor>()),
	_ecsEntity(std::make_shared<Entity>())
{
	InitEcs();
}

ZoneControllerManager::~ZoneControllerManager()
{
	if (_ecsEntity)
	{
		_ecsEntity->Dispose();
		_ecsEntity = nullptr;
	}
}

void ZoneControllerManager::InitEcs()
{
	_ecsEntity->AddComponent<ZoneManagerComponent>(std::make_shared<ZoneManagerComponent>());

	_ecsEntity->AddSystem<ZoneManagerMapSystem>(std::make_shared<ZoneManagerMapSystem>());
	_ecsEntity->AddSystem<ZoneManagerMessageDispatcher>(std::make_shared<ZoneManagerMessageDispatcher>());
	_ecsEntity->AddSystem<ZoneManagerUserSystem>(std::make_shared<ZoneManagerUserSystem>());
}

void ZoneControllerManager::Load()
{
	try
	{
		auto managerSys = _ecsEntity->GetSystem<ZoneManagerMapSystem>();
		if (managerSys != nullptr)
		{
			managerSys->LoadMaps(_ecsEntity);
		}
		
		_messageQueueProcessor->Start(
			[this](std::shared_ptr<IInternalMessage>  msg) {
				OnRecvHandleMessage(msg);
			});
	}
	catch (std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
	}
}


void ZoneControllerManager::Tick(int shardId, std::function<void(std::shared_ptr<ZoneController>)> fn)
{
	auto manager = _ecsEntity->GetComponent<ZoneManagerComponent>();
	if (manager == nullptr)
		return;

	if (shardId < 0 || shardId >= ConstInfo::ShardCount)
		return;

	auto& shard = manager->GetShardsByShardId(shardId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	for (auto& session : shard.sessions)
	{
		auto& zone = session.second;
		fn(zone);
	}

	if (_messageQueueProcessor)
		_messageQueueProcessor->Tick();
}


void ZoneControllerManager::EnqueueMessage(std::shared_ptr<IInternalMessage> msg)
{
	if (_messageQueueProcessor)
		_messageQueueProcessor->Enqueue(msg);
}

void ZoneControllerManager::OnRecvHandleMessage(std::shared_ptr<IInternalMessage> message)
{
	auto dispatcher = _ecsEntity->GetSystem<ZoneManagerMessageDispatcher>();
	if (dispatcher == nullptr)
		return;

	dispatcher->OnRecvHandleMessage(_ecsEntity, message);
}

