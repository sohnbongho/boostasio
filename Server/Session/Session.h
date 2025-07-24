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
	std::array<char, 1024> data_;

public :
	Session(tcp::socket socket);

	void Start();
	void Read();
	void Write(std::size_t length);
};