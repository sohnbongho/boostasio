#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>

using boost::asio::ip::tcp;
class Session : public std::enable_shared_from_this<Session>
{
private:
	tcp::socket _socket;
	std::array<char, 1024> _buffer;
	uint64_t _userId;

public :
	Session(tcp::socket socket);

	void Start();
	void OnRead();
	void Write(std::size_t length);

	uint64_t GetId() const;
	void SetId(uint64_t id);
	void Send(const std::string& message);
	void OnDisconnected();
};