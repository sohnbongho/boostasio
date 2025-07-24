#pragma once
#include <atomic>
#include <vector>
#include <memory>

class UserWorkerThread;
class UserSession;

class UserThreadPoolManager
{
public:
    UserThreadPoolManager(int threadCount);

    void Start();
    void Stop();
    void AddSession(std::shared_ptr<UserSession> session);   

private:
    int _threadCount;
    std::atomic<int> _nextIndex;
    std::vector<std::unique_ptr<UserWorkerThread>> _workers;
};

