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
    MonsterThreadPool(size_t threadCount);
    void Start();
    void Stop();
    void AddMonster(std::shared_ptr<MonsterObject> monster);
    void RemoveMonster(uint64_t id);
    void PostCommand(std::function<void()> cmd);
    std::shared_ptr<MonsterObject> FindMonster(uint64_t id);    

private:
    void ProcessCommands();
    void TickAll();    

    std::atomic<bool> _running;
    size_t _threadCount;
    std::vector<std::thread> _threads;
    std::unordered_map<uint64_t, std::shared_ptr<MonsterObject>> _monsters;
    std::mutex _mutex;

    MonsterCommandQueue _commandQueue;
};