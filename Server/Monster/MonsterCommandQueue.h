#pragma once
#include <queue>
#include <mutex>
#include <functional>

class MonsterCommandQueue
{
public:
    void Push(std::function<void()> cmd)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(std::move(cmd));
    }

    bool Pop(std::function<void()>& cmd)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_queue.empty()) return false;
        cmd = std::move(_queue.front());
        _queue.pop();
        return true;
    }

private:
    std::queue<std::function<void()>> _queue;
    std::mutex _mutex;
};