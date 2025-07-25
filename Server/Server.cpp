#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "./Session/AsyncTcpServer.h"

int main()
{
    try
    {
        const int threadCount = std::thread::hardware_concurrency();
        boost::asio::io_context io_context;

        // work guard 유지
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard(io_context.get_executor());

        AsyncTcpServer server(io_context, 12345);
        std::cout << "[Server] Started on port 12345" << std::endl;

        // 스레드풀 구성
        std::vector<std::thread> threadPool;
        for (int i = 0; i < threadCount; ++i)
        {
            threadPool.emplace_back([&io_context]() {
                io_context.run();
                });
        }

        for (auto& t : threadPool)
        {
            if (t.joinable()) t.join();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "예외 발생: " << e.what() << std::endl;
    }
    return 0;
}
