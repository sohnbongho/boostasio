#include "UserSession.h"
#include <iostream>

UserSession::UserSession(boost::asio::ip::tcp::socket&& socket)
    : _socket(std::move(socket)),
    _strand(boost::asio::make_strand(_socket.get_executor()))
{
}

void UserSession::Start()
{
    std::cout << "[Session] Started (ID: " << _id << ")" << std::endl;
    DoRead();
}

void UserSession::DoRead()
{
    auto self = shared_from_this();
    _socket.async_read_some(
        boost::asio::buffer(_buffer),
        boost::asio::bind_executor(_strand, [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::string received(_buffer.data(), length);
                    std::cout << "[Recv] " << received << std::endl;

                    Send("Echo: " + received);
                    DoRead();
                }
                else
                {
                    std::cout << "[Session] Disconnected (ID: " << _id << ")" << std::endl;
                }
            }));
}

void UserSession::Send(const std::string& msg)
{
    auto self = shared_from_this();
    boost::asio::post(_strand, [this, self, msg]()
        {
            bool writing = !_sendQueue.empty();
            _sendQueue.push_back(msg);

            if (!writing)
            {
                DoWrite(_sendQueue.front());
            }
        });
}

void UserSession::DoWrite(const std::string& msg)
{
    auto self = shared_from_this();
    boost::asio::async_write(_socket, boost::asio::buffer(msg),
        boost::asio::bind_executor(_strand, [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    _sendQueue.pop_front();
                    if (!_sendQueue.empty())
                    {
                        DoWrite(_sendQueue.front());
                    }
                }
            }));
}

void UserSession::Tick()
{
    std::cout << "[Tick] Session " << _id << std::endl;
}
