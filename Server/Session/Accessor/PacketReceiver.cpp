#include "PacketReceiver.h"
#include <iostream>

const int MaxReceiveSize = 64 * 1024;

PacketReceiver::PacketReceiver(boost::asio::ip::tcp::socket& socket,
    boost::asio::strand<boost::asio::io_context::executor_type>& strand)
    : _socket(socket), _strand(strand), _readHeader{}
{
}
PacketReceiver::~PacketReceiver() 
{
    std::cout << "~PacketReceiver() " << std::endl;
}

void PacketReceiver::Start(MessageHandler onMessage, DisconnectHandler onDisconnect)
{
    _onMessage = std::move(onMessage);
    _onDisconnect = std::move(onDisconnect);

    ReadHeader();
}

void PacketReceiver::ReadHeader()
{
    auto self = shared_from_this();

    boost::asio::async_read(_socket, boost::asio::buffer(_readHeader),
        boost::asio::bind_executor(_strand,
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (ec)
                {
                    if (_onDisconnect)
                        _onDisconnect();
                    return;
                }

                uint32_t bodySize = 0;
                std::memcpy(&bodySize, _readHeader.data(), 4);
                boost::endian::big_to_native_inplace(bodySize);

                if (bodySize == 0 || bodySize > MaxReceiveSize)
                {
                    std::cerr << "[Recv] Invalid body size: " << bodySize << "\n";
                    if (_onDisconnect)
                        _onDisconnect();
                    return;
                }

                _readBody.resize(bodySize);
                ReadBody(bodySize);
            }));
}

void PacketReceiver::ReadBody(std::size_t bodySize)
{
    auto self = shared_from_this();

    boost::asio::async_read(_socket, boost::asio::buffer(_readBody),
        boost::asio::bind_executor(_strand,
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (ec)
                {
                    if (_onDisconnect)
                        _onDisconnect();
                    return;
                }

                Messages::MessageWrapper wrapper;
                if (!wrapper.ParseFromArray(_readBody.data(), static_cast<int>(_readBody.size())))
                {
                    std::cerr << "[Recv] Failed to parse protobuf message\n";
                }
                else
                {
                    if (_onMessage)
                        _onMessage(wrapper);
                }

                ReadHeader(); // 다음 메시지 계속 수신
            }));
}

void PacketReceiver::Stop()
{
    _onMessage = nullptr;
    _onDisconnect = nullptr;
}