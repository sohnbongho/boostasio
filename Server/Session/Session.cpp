
#include "Session.h"

 Session::Session(tcp::socket socket)
    : _socket(std::move(socket))
{
}


void Session::Start()
{
    Read();
}

void Session::Read() 
{
    auto self = shared_from_this();
    _socket.async_read_some(
        boost::asio::buffer(data_),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                Write(length);
            }
        });
}

void Session::Write(std::size_t length)
{
    auto self = shared_from_this();
    boost::asio::async_write(
        _socket,
        boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                Read(); // 다시 읽기 대기
            }
        });
}

