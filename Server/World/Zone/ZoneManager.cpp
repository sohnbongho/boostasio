#include "ZoneManager.h"

ZoneManager& ZoneManager::Instance()
{
	static ZoneManager instance;
	return instance;
}

ZoneManager::ZoneManager()
	: _zoneId(0),
	_messageQueueProcessor(std::make_shared< MessageQueueProcessor>())
{
}

ZoneManager::~ZoneManager()
{

}


ZoneManager::Shard& ZoneManager::GetShard(int zoneId)
{
	return _shards[zoneId % ShardCount];
}

void ZoneManager::Load()
{
	try
	{
		{
			int mapId = 1001;
			int zoneId = GenerateId();
			auto zone = std::make_shared<ZoneController>(zoneId, mapId);
			zone->Load();
			Add(zoneId, zone);
		}
		{
			int mapId = 1002;
			int zoneId = GenerateId();
			auto zone = std::make_shared<ZoneController>(zoneId, mapId);
			zone->Load();
			Add(zoneId, zone);
		}
		{
			int mapId = 1003;
			int zoneId = GenerateId();
			auto zone = std::make_shared<ZoneController>(zoneId, mapId);
			zone->Load();
			Add(zoneId, zone);
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


void ZoneManager::Add(int zoneId, std::shared_ptr<ZoneController> zone)
{
	auto& shard = GetShard(zoneId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	shard.sessions[zoneId] = zone;
}


void ZoneManager::Tick(int shardId, std::function<void(std::shared_ptr<ZoneController>)> fn)
{
	if (shardId < 0 || shardId >= ShardCount)
		return;

	auto& shard = _shards[shardId];
	std::lock_guard<std::mutex> lock(shard.mutex);
	for (auto& session : shard.sessions)
	{
		auto& zone = session.second;
		fn(zone);
	}

	if (_messageQueueProcessor)
		_messageQueueProcessor->Tick();
}

std::shared_ptr<ZoneController> ZoneManager::GetZone(int zoneId)
{
	auto& shard = GetShard(zoneId);
	std::lock_guard<std::mutex> lock(shard.mutex);
	return shard.sessions[zoneId];
}



void ZoneManager::EnqueueMessage(std::shared_ptr<IInternalMessage> msg)
{
	if (_messageQueueProcessor)
		_messageQueueProcessor->Enqueue(std::move(msg));
}

void ZoneManager::OnRecvHandleMessage(std::shared_ptr<IInternalMessage> message)
{
	std::cout << "OnRecvHandleMessage type:" << message->GetMessageType() << std::endl;
}