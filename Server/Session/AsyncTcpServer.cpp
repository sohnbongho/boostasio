#include "AsyncTcpServer.h"
#include "UserSession.h"
#include <iostream>

using boost::asio::ip::tcp;

AsyncTcpServer::AsyncTcpServer(boost::asio::io_context& io_context, short port)
    : _io_context(io_context),
    _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
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
                session->SetId(rand()); // 유저 ID 할당 (임시)
                session->Start();
            }
            Accept();
        });
}
