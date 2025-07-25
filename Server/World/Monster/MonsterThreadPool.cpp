
#include "MonsterThreadPool.h"

MonsterThreadPool::MonsterThreadPool(size_t threadCount)
	: _running(false), _threadCount(threadCount)
{}

void MonsterThreadPool::Start()
{
	_running = true;
	for (size_t i = 0; i < _threadCount; ++i)
	{
		_threads.emplace_back([this]() {
			while (_running)
			{
				ProcessCommands();
				TickAll();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			});
	}
}

void MonsterThreadPool::Stop()
{
	_running = false;
	for (auto& t : _threads)
	{
		if (t.joinable()) t.join();
	}
}

void MonsterThreadPool::AddMonster(std::shared_ptr<MonsterObject> monster)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_monsters[monster->GetId()] = monster;
}

void MonsterThreadPool::RemoveMonster(uint64_t id)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_monsters.erase(id);
}

void MonsterThreadPool::PostCommand(std::function<void()> cmd)
{
	_commandQueue.Push(std::move(cmd));
}

std::shared_ptr<MonsterObject> MonsterThreadPool::FindMonster(uint64_t id)
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _monsters.find(id);
	return (it != _monsters.end()) ? it->second : nullptr;
}


void MonsterThreadPool::ProcessCommands()
{
	std::function<void()> cmd;
	while (_commandQueue.Pop(cmd))
	{
		cmd();
	}
}

void MonsterThreadPool::TickAll()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto& kv : _monsters)
	{
		auto& id = kv.first;
		auto& monster = kv.second;
		monster->Tick();
	}
}