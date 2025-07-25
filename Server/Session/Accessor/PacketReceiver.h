#pragma once

#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <array>
#include <vector>
#include <functional>
#include <memory>
#include "../../DTO/message.pb.h" // MessageWrapper ��ο� �°� �����ϼ���

class PacketReceiver : public std::enable_shared_from_this<PacketReceiver>
{
public:
    using MessageHandler = std::function<void(const Messages::MessageWrapper&)>;
    using DisconnectHandler = std::function<void()>;

    PacketReceiver(boost::asio::ip::tcp::socket& socket,
        boost::asio::strand<boost::asio::io_context::executor_type>& strand);
    ~PacketReceiver();

    void Start(MessageHandler onMessage, DisconnectHandler onDisconnect);
    void Stop();

private:
    void ReadHeader();
    void ReadBody(std::size_t bodySize);

private:
    boost::asio::ip::tcp::socket& _socket;
    boost::asio::strand<boost::asio::io_context::executor_type>& _strand;

    std::array<char, 4> _readHeader;
    std::vector<char> _readBody;

    MessageHandler _onMessage;
    DisconnectHandler _onDisconnect;
};
