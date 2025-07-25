


#include "DummyTcpClient.h"

DummyTcpClient::DummyTcpClient(boost::asio::io_context& io_context, const std::string& host, uint16_t port)
	: _socket(io_context), _buffer{}
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

void DummyTcpClient::Send(std::shared_ptr<std::string> msg)
{
	boost::asio::async_write(_socket, boost::asio::buffer(*msg),
		[msg](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (ec)
				std::cerr << "[Client] Send failed: " << ec.message() << "\n";
		});
}

void DummyTcpClient::SendProto(const Messages::MessageWrapper& wrapper)
{
	//// 1. 직렬화
	std::string serializedData;
	if (!wrapper.SerializeToString(&serializedData))
	{
		std::cerr << "[Client] Failed to serialize protobuf message.\n";
		return;
	}

	// 2. 메시지 길이 prefix 추가
	uint32_t size = static_cast<uint32_t>(serializedData.size());
	std::string packet;
	packet.resize(4); // 4바이트 길이 공간
	packet[0] = (size >> 0) & 0xFF;
	packet[1] = (size >> 8) & 0xFF;
	packet[2] = (size >> 16) & 0xFF;
	packet[3] = (size >> 24) & 0xFF;
	packet += serializedData;

	// 3. 전송
	auto data = std::make_shared<std::string>(std::move(packet));
	boost::asio::async_write(_socket, boost::asio::buffer(*data),
		[data](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (ec)
				std::cerr << "[Client] Send failed: " << ec.message() << "\n";
		});
}


void DummyTcpClient::DoRead()
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