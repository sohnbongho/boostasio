// async_tcp_server.cpp
#include "AsyncTcpServer.h"
#include "Session.h"
#include "UserSessionManager.h"

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
                UserSessionManager::Instance().CreateAndStartSession(std::move(socket));
            }
            Accept(); // 계속 다음 클라이언트 받기
        });
}
