// async_tcp_server.cpp
#include "AsyncTcpServer.h"
#include "Session.h"

AsyncTcpServer::AsyncTcpServer(boost::asio::io_context& io_context, short port)
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    Accept();
}
void AsyncTcpServer::Accept()
{
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->Start();
            }
            Accept(); // 계속 다음 클라이언트 받기
        });
}
