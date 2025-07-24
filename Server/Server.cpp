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
        const int threadCount = std::thread::hardware_concurrency(); // 코어 수 기준
        boost::asio::io_context io_context;
        
        // work guard를 걸어줘야 io_context.run()이 바로 return되지 않음
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard(io_context.get_executor());

        AsyncTcpServer server(io_context, 12345);
        std::cout << "[Server] 비동기 TCP 서버 시작 (포트 12345)" << std::endl;
        io_context.run(); // 이벤트 루프 시작

        // thread pool 구성
        std::vector<std::thread> threadPool;
        for (int i = 0; i < threadCount; ++i)
        {
            threadPool.emplace_back([&io_context]() {
                io_context.run(); // 각 스레드에서 run()
                });
        }

        // 메인 스레드는 여기서 기다림
        for (auto& thread : threadPool)
        {
            if (thread.joinable())
                thread.join();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "예외 발생: " << e.what() << std::endl;
    }
    return 0;
}
