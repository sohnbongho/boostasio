#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

class DummyTcpClient
{
public:
	DummyTcpClient(boost::asio::io_context& io_context, const std::string& host, uint16_t port)
		: _socket(io_context)
	{
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(host, std::to_string(port));

		boost::asio::async_connect(_socket, endpoints,
			[this](boost::system::error_code ec, tcp::endpoint)
			{
				if (!ec)
				{
					std::cout << "[Client] Connected to server.\n";
					auto data = std::make_shared<std::string>("Hello Server!\n");
					Send(data);
					DoRead();
				}
				else
				{
					std::cerr << "[Client] Connect failed: " << ec.message() << "\n";
				}
			});
	}

	void Send(std::shared_ptr<std::string> msg)
	{
		boost::asio::async_write(_socket, boost::asio::buffer(*msg),
			[msg](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (ec)
					std::cerr << "[Client] Send failed: " << ec.message() << "\n";
			});
	}

private:
	void DoRead()
	{
		_socket.async_read_some(boost::asio::buffer(_buffer),
			[this](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::string msg(_buffer.data(), length);
					std::cout << "[Client] Received: " << msg << "\n";
					DoRead(); // 계속 수신
				}
				else
				{
					std::cerr << "[Client] Read failed: " << ec.message() << "\n";
				}
			});
	}

private:
	tcp::socket _socket;
	std::array<char, 1024> _buffer;
};
