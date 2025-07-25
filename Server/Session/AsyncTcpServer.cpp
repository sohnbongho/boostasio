#include "AsyncTcpServer.h"
#include "UserSession.h"
#include <iostream>
#include "UserSessionManager.h"

using boost::asio::ip::tcp;

AsyncTcpServer::AsyncTcpServer(IoContextPool& ioPool, short port)
    : _ioPool(ioPool),
    _acceptor(ioPool.GetNextIoContext(), tcp::endpoint(tcp::v4(), port))
{
    Accept();
}

void AsyncTcpServer::Accept()
{
    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                auto session = std::make_shared<UserSession>(std::move(socket));
                uint64_t id = UserSessionManager::Instance().AddSession(session);
                std::cout << "[Server] User connected. ID: " << id << std::endl;
                session->Start();
            }
            Accept();
        });
}
