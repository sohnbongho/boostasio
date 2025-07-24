#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include "UserSession.h"

class UserSessionManager
{
public:
    static UserSessionManager& Instance();
        
    void RemoveSession(uint64_t sessionId);
    std::shared_ptr<UserSession> GetSession(uint64_t sessionId);

    void Broadcast(const std::string& message);
    uint64_t CreateAndStartSession(boost::asio::ip::tcp::socket socket);

private:
    UserSessionManager() = default;
    uint64_t GenerateId();

    std::unordered_map<uint64_t, std::shared_ptr<UserSession>> _sessions;
    std::mutex _mutex;
    

    std::atomic<uint64_t> _idCounter{ 1 }; // ID ½ÃÀÛ°ª
};
