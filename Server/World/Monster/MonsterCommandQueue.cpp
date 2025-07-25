#include "MonsterCommandQueue.h"

void MonsterCommandQueue::Push(std::function<void()> cmd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(std::move(cmd));
}

bool MonsterCommandQueue::Pop(std::function<void()>& cmd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty()) return false;
    cmd = std::move(_queue.front());
    _queue.pop();
    return true;
}
