#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <memory>
#include <atomic>

class IoContextPool
{
public:
    explicit IoContextPool(size_t poolSize);

    void Run();
    void Stop();
    boost::asio::io_context& GetNextIoContext();

private:
    std::vector<std::shared_ptr<boost::asio::io_context>> _ioContexts;
    std::vector<std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>> _workGuards;
    std::vector<std::thread> _threads;
    std::atomic<size_t> _nextIndex;
};