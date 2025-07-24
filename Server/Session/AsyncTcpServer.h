#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>

using boost::asio::ip::tcp;

class AsyncTcpServer
{
public:
    explicit AsyncTcpServer(boost::asio::io_context& io_context, short port);

private:
    tcp::acceptor _acceptor;

    void Accept();
};