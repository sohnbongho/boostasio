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
	explicit IoContextPool(std::size_t poolSize);
	void Run();
	void Stop();
	boost::asio::io_context& GetNextIoContext();

private:
	std::vector<std::shared_ptr<boost::asio::io_context>> _ioContexts;
	std::vector<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> _workGuards; // ✅ work guard
	std::vector<std::thread> _threads;
	std::atomic<std::size_t> _nextIndex = 0;
};
