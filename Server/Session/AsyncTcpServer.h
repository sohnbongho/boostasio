#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "IoContextPool.h"

class AsyncTcpServer
{
public:
    AsyncTcpServer(IoContextPool& ioPool, short port);

private:
    void Accept();

    boost::asio::ip::tcp::acceptor _acceptor;
    IoContextPool& _ioPool;
};
