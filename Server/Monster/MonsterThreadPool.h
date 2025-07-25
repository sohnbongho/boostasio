#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <chrono>
#include "MonsterObject.h"
#include "MonsterCommandQueue.h"

class MonsterThreadPool
{
public:
    MonsterThreadPool(size_t threadCount)
        : _running(false), _threadCount(threadCount)
    {}

    void Start()
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

    void Stop()
    {
        _running = false;
        for (auto& t : _threads)
        {
            if (t.joinable()) t.join();
        }
    }

    void AddMonster(std::shared_ptr<MonsterObject> monster)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _monsters[monster->GetId()] = monster;
    }

    void RemoveMonster(uint64_t id)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _monsters.erase(id);
    }

    void PostCommand(std::function<void()> cmd)
    {
        _commandQueue.Push(std::move(cmd));
    }

    std::shared_ptr<MonsterObject> FindMonster(uint64_t id)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _monsters.find(id);
        return (it != _monsters.end()) ? it->second : nullptr;
    }

private:
    void ProcessCommands()
    {
        std::function<void()> cmd;
        while (_commandQueue.Pop(cmd))
        {
            cmd();
        }
    }

    void TickAll()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (auto& kv : _monsters)
        {
            auto& id = kv.first;
            auto& monster = kv.second;
            monster->Tick();
        }
    }

    std::atomic<bool> _running;
    size_t _threadCount;
    std::vector<std::thread> _threads;
    std::unordered_map<uint64_t, std::shared_ptr<MonsterObject>> _monsters;
    std::mutex _mutex;

    MonsterCommandQueue _commandQueue;
};