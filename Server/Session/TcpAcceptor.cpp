#include "TcpAcceptor.h"
#include "../Session/User/UserSession.h"
#include <iostream>
#include "./Manager/UserSessionShardManager.h"

using boost::asio::ip::tcp;

TcpAcceptor::TcpAcceptor(IoContextPool& ioPool, short port)
    : _ioPool(ioPool),
    _acceptor(ioPool.GetNextIoContext(), tcp::endpoint(tcp::v4(), port))
{
    std::cout << "[Server] Acceptor:" << std::boolalpha << _acceptor.is_open() << std::endl;
    Accept();
}

void TcpAcceptor::Accept()
{
    std::cout << "[Accept] Waiting for connection..." << std::endl;

    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
        {
            std::cout << "[Accept] Handler entered" << std::endl;

            if (!ec)
            {
                std::cout << "[Accept] Socket accepted!" << std::endl;
                uint64_t sessionId = UserSessionShardManager::Instance().GenerateId();
                boost::asio::io_context& ctx = _ioPool.GetNextIoContext();

                auto session = std::make_shared<UserSession>(sessionId, std::move(socket), ctx);
                UserSessionShardManager::Instance().AddSession(sessionId, session);
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