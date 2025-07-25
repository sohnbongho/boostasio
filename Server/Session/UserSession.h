#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <deque>
#include <array>
#include <mutex>

class UserSession : public std::enable_shared_from_this<UserSession>
{
public:
    explicit UserSession(boost::asio::ip::tcp::socket&& socket);

    void Start();
    void Send(const std::string& msg);
    void HandleCommand(const std::string& command);

private:
    void DoRead();
    void DoWrite(const std::string& msg);

    boost::asio::ip::tcp::socket _socket;
    boost::asio::strand<boost::asio::any_io_executor> _strand;
    std::array<char, 1024> _buffer;
    std::deque<std::string> _sendQueue;
};