
#include <boost/endian/conversion.hpp> // 필수

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
				//auto data = std::make_shared<std::string>("Hello Server!\n");
				// 메시지 보내기
				{
					auto messageWrapper = std::make_shared<Messages::MessageWrapper>();
					auto* rquest = messageWrapper->mutable_connected_response();
					rquest->set_index(1);

					SendProto(messageWrapper);
				}
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

void DummyTcpClient::SendProto(const std::shared_ptr<Messages::MessageWrapper> msg)
{
	std::string payload;
	if (!msg->SerializeToString(&payload))
	{
		std::cerr << "[Client] Failed to serialize.\n";
		return;
	}

	uint32_t size = static_cast<uint32_t>(payload.size());
	boost::endian::native_to_big_inplace(size); // 네트워크 바이트 순서

	// 전체 메시지 = [4바이트 크기] + [protobuf payload]
	std::string fullMessage(reinterpret_cast<char*>(&size), sizeof(size));
	fullMessage += payload;

	auto sendData = std::make_shared<std::string>(std::move(fullMessage));

	boost::asio::async_write(_socket, boost::asio::buffer(*sendData),
		[sendData](boost::system::error_code ec, std::size_t)
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