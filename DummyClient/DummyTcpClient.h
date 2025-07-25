#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "./DTO/message.pb.h"




using boost::asio::ip::tcp;

class DummyTcpClient
{
public:
	DummyTcpClient(boost::asio::io_context& io_context, const std::string& host, uint16_t port);
	void Send(std::shared_ptr<std::string> msg);	
	void SendProto(const Messages::MessageWrapper& wrapper);

private:
	void DoRead();	

private:
	tcp::socket _socket;
	std::array<char, 1024> _buffer;
};
