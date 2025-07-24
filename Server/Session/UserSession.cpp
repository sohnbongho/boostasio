// Session.cpp
#include "UserSession.h"
#include <iostream>

UserSession::UserSession(boost::asio::io_context& io_context)
    : _socket(io_context),
    _strand(io_context.get_executor())
{
}

void UserSession::Start()
{
    std::cout << "[Session] Start ID: " << _id << std::endl;
    DoRead();
}

void UserSession::DoRead()
{
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(_buffer),
        boost::asio::bind_executor(_strand, [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec)
            {
                std::string received(_buffer.data(), length);
                std::cout << "[Recv] " << received << std::endl;

                Send("Echo: " + received); // Echo
                DoRead();
            }
            }));
}

void UserSession::Send(const std::string& msg)
{
    auto self = shared_from_this();
    boost::asio::post(_strand, [this, self, msg]() {
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
        boost::asio::bind_executor(_strand, [this, self](boost::system::error_code ec, std::size_t /*length*/) {
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
    std::cout << "[Tick] Session: " << _id << std::endl;
}

uint64_t UserSession::GetId() const { return _id; }
void UserSession::SetId(uint64_t id) { _id = id; }
