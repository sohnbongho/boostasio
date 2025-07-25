#pragma once
#include <boost/asio.hpp>
#include <memory>

class AsyncTcpServer
{
public:
    AsyncTcpServer(boost::asio::io_context& io_context, short port);

private:
    void Accept();

    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::io_context& _io_context;
};
