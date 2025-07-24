#include "UserThreadPoolManager.h"
#include "UserWorkerThread.h"
#include "../Session/UserSession.h"


UserThreadPoolManager::UserThreadPoolManager(int threadCount)
    : _threadCount(threadCount), _nextIndex(0)
{
    for (int i = 0; i < _threadCount; ++i)
        _workers.emplace_back(std::make_unique<UserWorkerThread>());
}

void UserThreadPoolManager::Start() 
{
    for (auto& worker : _workers)
        worker->Start();
}


void UserThreadPoolManager::Stop()
{
    for (auto& worker : _workers)
        worker->Stop();
}

void UserThreadPoolManager::AddSession(std::shared_ptr<UserSession> session)
{
    // 라운드로빈 분배
    auto& worker = _workers[_nextIndex++ % _threadCount];
    worker->AddSession(session);
}