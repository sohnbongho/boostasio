#include "UserSessionManager.h"

UserSessionManager& UserSessionManager::Instance()
{
    static UserSessionManager instance;
    return instance;
}

uint64_t UserSessionManager::GenerateId()
{
    return _idCounter++;
}


void UserSessionManager::RemoveSession(uint64_t sessionId)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.erase(sessionId);
}

std::shared_ptr<UserSession> UserSessionManager::GetSession(uint64_t sessionId)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _sessions.find(sessionId);
    return (it != _sessions.end()) ? it->second : nullptr;
}

void UserSessionManager::Broadcast(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_mutex);
    /*for (auto& [id, session] : _sessions)
    {
        if (session)
        {
            session->Send(message);
        }
    }*/
}

uint64_t UserSessionManager::CreateAndStartSession(boost::asio::ip::tcp::socket socket)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto session = std::make_shared<UserSession>(std::move(socket));
    auto id = GenerateId();
    session->SetId(id);
    _sessions[id] = session;
    session->Start(); // 여기서 시작
    return id;
}
