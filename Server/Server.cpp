// async_tcp_server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include "Session/AsyncTcpServer.h"

using boost::asio::ip::tcp;




int main()
{
    try
    {
        boost::asio::io_context io_context;
        AsyncTcpServer server(io_context, 12345);
        std::cout << "[Server] 비동기 TCP 서버 시작 (포트 12345)" << std::endl;
        io_context.run(); // 이벤트 루프 시작
    }
    catch (std::exception& e)
    {
        std::cerr << "예외 발생: " << e.what() << std::endl;
    }
    return 0;
}
