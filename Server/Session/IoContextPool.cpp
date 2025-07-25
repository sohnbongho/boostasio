#include "IoContextPool.h"

IoContextPool::IoContextPool(size_t poolSize)
    : _nextIndex(0)
{
    for (size_t i = 0; i < poolSize; ++i)
    {
        auto ctx = std::make_shared<boost::asio::io_context>();
        auto guard = std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(ctx->get_executor());
        _ioContexts.push_back(ctx);
        _workGuards.push_back(std::move(guard));
    }
}

void IoContextPool::Run()
{
    for (auto& ctx : _ioContexts)
    {
        _threads.emplace_back([ctx]() {
            ctx->run();
        });
    }
}

void IoContextPool::Stop()
{
    for (auto& ctx : _ioContexts)
        ctx->stop();

    for (auto& t : _threads)
        if (t.joinable()) t.join();
}

boost::asio::io_context& IoContextPool::GetNextIoContext()
{
    return *_ioContexts[_nextIndex++ % _ioContexts.size()];
}