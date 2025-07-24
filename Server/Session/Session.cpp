
#include "Session.h"
#include "UserSessionManager.h"

 Session::Session(tcp::socket socket)
    : _socket(std::move(socket)), _buffer{}, _userId(0)
{     
}


void Session::Start()
{
    OnRead();
}

void Session::OnRead() 
{
    auto self = shared_from_this();
    _socket.async_read_some(
        boost::asio::buffer(_buffer),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                OnRead();
            }
            else
            {
                OnDisconnected(); // ²÷±è Ã³¸®
            }
        });
}

void Session::Write(std::size_t length)
{
    auto self = shared_from_this();
    boost::asio::async_write(
        _socket,
        boost::asio::buffer(_buffer, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                OnRead(); // ´Ù½Ã ÀÐ±â ´ë±â
            }            
        });
}

uint64_t Session::GetId() const
{
    return _userId;
}

void Session::SetId(uint64_t id)
{
    _userId = id;
}

void Session::Send(const std::string& message)
{

}

void Session::OnDisconnected()
{
    std::cout << "Session disconnected: " << _userId << std::endl;
    UserSessionManager::Instance().RemoveSession(_userId);
}