#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <deque>
#include <mutex>

class UserSession : public std::enable_shared_from_this<UserSession>
{
public:
    explicit UserSession(boost::asio::ip::tcp::socket&& socket);

    void Start();
    void Send(const std::string& msg);
    void Tick(); // 유저 상태 갱신 등

    uint64_t GetId() const { return _id; }
    void SetId(uint64_t id) { _id = id; }

private:
    void DoRead();
    void DoWrite(const std::string& msg);

    boost::asio::ip::tcp::socket _socket;
    boost::asio::strand<boost::asio::any_io_executor> _strand;  // 최신 any_io_executor 사용
    std::array<char, 1024> _buffer;

    std::deque<std::string> _sendQueue;
    std::mutex _writeMutex;

    uint64_t _id = 0;
};
