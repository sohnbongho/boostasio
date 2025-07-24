#include "UserWorkerThread.h"

void UserWorkerThread::AddSession(std::shared_ptr<UserSession> session)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.emplace_back(session);
}

void UserWorkerThread::Start()
{
    _thread = std::thread([this]() {
        while (_running)
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                for (auto& session : _sessions)
                    session->Tick();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        });
}
void UserWorkerThread::Stop()
{
    _running = false;
    if (_thread.joinable())
        _thread.join();
}

