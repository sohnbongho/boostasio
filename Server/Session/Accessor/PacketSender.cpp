#include "PacketSender.h"
#include <boost/endian/conversion.hpp>
#include <iostream>

PacketSender::PacketSender(boost::asio::ip::tcp::socket& socket,
    boost::asio::strand<boost::asio::io_context::executor_type>& strand)
    : _socket(socket), _strand(strand)
{
}
PacketSender::~PacketSender()
{
    std::cout << "~PacketSender() " << std::endl;
}

void PacketSender::Send(const std::string& plainText)
{
    if (_stopped)
        return;

    auto self = shared_from_this();
    auto msg = std::make_shared<std::string>(plainText);

    boost::asio::post(_strand, [this, self, msg]() {
        bool idle = _sendQueue.empty();
        _sendQueue.push_back(msg);
        if (idle)
            DoWrite();
        });
}

void PacketSender::Send(const Messages::MessageWrapper& protoMsg)
{
    if (_stopped)
        return;

    auto self = shared_from_this();

    std::string serialized;
    if (!protoMsg.SerializeToString(&serialized))
    {
        std::cerr << "[Sender] Failed to serialize protobuf\n";
        return;
    }

    uint32_t bodySize = static_cast<uint32_t>(serialized.size());
    boost::endian::native_to_big_inplace(bodySize); // 호스트 → 네트워크 byte order

    auto fullPacket = std::make_shared<std::string>();
    fullPacket->resize(4 + serialized.size());
    std::memcpy(&(*fullPacket)[0], &bodySize, 4);
    std::memcpy(&(*fullPacket)[4], serialized.data(), serialized.size());

    boost::asio::post(_strand, [this, self, fullPacket]() {
        
        if (_stopped)
            return;

        bool idle = _sendQueue.empty();
        _sendQueue.push_back(fullPacket);
        if (idle)
            DoWrite();
        });
}

void PacketSender::DoWrite()
{
    auto self = shared_from_this();

    boost::asio::async_write(_socket, boost::asio::buffer(*_sendQueue.front()),
        boost::asio::bind_executor(_strand,
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (_stopped)
                    return;

                if (ec)
                {
                    std::cerr << "[Sender] Write error: " << ec.message() << "\n";
                    return;
                }

                _sendQueue.pop_front();
                if (!_sendQueue.empty())
                    DoWrite();
            }));
}


void PacketSender::Stop()
{
    _stopped = true;

    // 큐 클리어
    _sendQueue.clear();

    // 전송 중지 (람다 내부 참조 해제용)
    _onWriteComplete = nullptr;
}
