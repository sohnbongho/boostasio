#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <string>
#include <functional>
#include "../../DTO/message.pb.h"


class PacketSender : public std::enable_shared_from_this<PacketSender>
{
public:
    PacketSender(boost::asio::ip::tcp::socket& socket,
        boost::asio::strand<boost::asio::io_context::executor_type>& strand);
    ~PacketSender();

    void Send(const std::string& plainText); // 일반 문자열
    void Send(const Messages::MessageWrapper& protoMsg); // protobuf 메시지
    void Stop();

private:
    void DoWrite();

private:
    boost::asio::ip::tcp::socket& _socket;
    boost::asio::strand<boost::asio::io_context::executor_type>& _strand;

    std::deque<std::shared_ptr<std::string>> _sendQueue;
    bool _isWriting = false;

    bool _stopped = false;
    std::function<void()> _onWriteComplete;
};
