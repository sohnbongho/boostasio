#pragma once
#include <queue>
#include <mutex>
#include <functional>

class MonsterCommandQueue
{
public:
	void Push(std::function<void()> cmd);
	bool Pop(std::function<void()>& cmd);

private:
	std::queue<std::function<void()>> _queue;
	std::mutex _mutex;
};