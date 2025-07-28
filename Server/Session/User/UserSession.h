#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>

#include "../Accessor/PacketReceiver.h"
#include "../Accessor/PacketSender.h"
#include "../../DTO/message.pb.h"

class UserSession : public std::enable_shared_from_this<UserSession>
{
public:
    UserSession(uint64_t sessionId, boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& context);
    ~UserSession();

    void StartSession();
    void Send(const std::string& msg);

    void Tick();

private:
    void OnDisconnected();
    void HandleMessage(const Messages::MessageWrapper& msg);

private:
    uint64_t _sessionId;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;

    std::shared_ptr<PacketReceiver> _receiver;
    std::shared_ptr<PacketSender> _sender;
};
