
#include "UserSession.h"
#include "UserSessionManager.h"

 UserSession::UserSession(tcp::socket socket)
    : _socket(std::move(socket)), _buffer{}, _id(0)
{     
}


void UserSession::Start()
{
    OnRead();
}

void UserSession::OnRead() 
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

void UserSession::Write(std::size_t length)
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

void UserSession::OnDisconnected()
{
    std::cout << "Session disconnected: " << _id << std::endl;
    UserSessionManager::Instance().RemoveSession(_id);
}

uint64_t UserSession::GetId() const
{
    return _id;
}

void UserSession::SetId(uint64_t id)
{
    _id = id;
}

void UserSession::Send(const std::string& message)
{

}



void UserSession::Tick()
{
    std::cout << "[Tick] " << _id << std::endl;
}
