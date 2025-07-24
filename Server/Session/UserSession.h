#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <array>
#include <deque>

using boost::asio::ip::tcp;
class UserSession : public std::enable_shared_from_this<UserSession>
{
public:
    UserSession(boost::asio::io_context& io_context);

    void Start();
    void Send(const std::string& msg);
    void Tick(); // 필요시 유저 상태 관리

    uint64_t GetId() const;
    void SetId(uint64_t id);

private:
    void DoRead();
    void DoWrite(const std::string& msg);

private:
    boost::asio::ip::tcp::socket _socket;
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    std::array<char, 1024> _buffer;

    std::mutex _writeMutex;
    std::deque<std::string> _sendQueue;

    uint64_t _id;
};