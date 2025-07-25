#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <memory>
#include <boost/asio/executor_work_guard.hpp>

class IoContextPool
{
public:
    explicit IoContextPool(std::size_t poolSize)
    {
        for (std::size_t i = 0; i < poolSize; ++i)
        {
            auto ctx = std::make_shared<boost::asio::io_context>();
            _ioContexts.push_back(ctx);
            _workGuards.emplace_back(boost::asio::make_work_guard(*ctx)); // ✅ 여기 추가
        }
    }

    void Run()
    {
        for (auto& ctx : _ioContexts)
        {
            _threads.emplace_back([ctx]() {
                std::cout << "[Run] Starting io_context: " << ctx.get() << std::endl;
                ctx->run();
                std::cout << "[Run] io_context ended\n";
                });
        }

        for (auto& t : _threads)
            t.join();
    }

    void Stop()
    {
        for (auto& ctx : _ioContexts)
        {
            ctx->stop();
        }
        for (auto& thread : _threads)
        {
            if (thread.joinable())
                thread.join();
        }
    }
    

    boost::asio::io_context& GetNextIoContext()
    {
        std::size_t index = _nextIndex.fetch_add(1) % _ioContexts.size(); 
        return *_ioContexts[index];
    }
    

private:
    std::vector<std::shared_ptr<boost::asio::io_context>> _ioContexts;
    std::vector<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> _workGuards; // ✅ work guard
    std::vector<std::thread> _threads;
    std::atomic<std::size_t> _nextIndex = 0;
};
