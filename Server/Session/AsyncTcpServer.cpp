#include "AsyncTcpServer.h"
#include "UserSession.h"
#include <iostream>
#include "UserSessionManager.h"

using boost::asio::ip::tcp;

AsyncTcpServer::AsyncTcpServer(IoContextPool& ioPool, short port)
    : _ioPool(ioPool),
    _acceptor(ioPool.GetNextIoContext(), tcp::endpoint(tcp::v4(), port))
{
    std::cout << "[Server] Acceptor open? " << std::boolalpha << _acceptor.is_open() << std::endl;
    Accept();
}

void AsyncTcpServer::Accept()
{
    std::cout << "[Accept] Waiting for connection..." << std::endl;

    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
        {
            std::cout << "[Accept] Handler entered" << std::endl;

            if (!ec)
            {
                std::cout << "[Accept] Socket accepted!" << std::endl;
                uint64_t sessionId = UserSessionManager::Instance().GenerateId();

                // 🎯 다른 io_context로 분산
                boost::asio::io_context& ctx = _ioPool.GetNextIoContext();

                auto session = std::make_shared<UserSession>(sessionId, std::move(socket), ctx);
                UserSessionManager::Instance().AddSession(sessionId, session);
                std::cout << "[Server] User connected. sessionId: " << sessionId << std::endl;
                session->StartSession();
            }
            else
            {
                std::cerr << "[Accept] Error: " << ec.message() << std::endl;
            }

            Accept(); // 재귀 호출
        });
}