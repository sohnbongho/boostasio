#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <array>
#include <memory>
#include <string>

class UserSession : public std::enable_shared_from_this<UserSession>
{
public:
    UserSession(uint64_t sessionId, boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& context);

    void StartSession();                       // 세션 시작 (read 시작)
    void Send(const std::string& msg);         // 클라이언트로 메시지 보내기
    void OnDisconnected();                     // 연결 종료 처리

    uint64_t GetSessionId() const { return _sessionId; }

private:
    void DoRead();                             // 내부 read 처리
    void DoWrite(const std::string& msg);      // 내부 write 처리
    void HandleCommand(const std::string& command); // 수신한 명령 처리

private:
    uint64_t _sessionId;

    boost::asio::ip::tcp::socket _socket;                         // 소켓
    boost::asio::strand<boost::asio::io_context::executor_type> _strand; // strand for thread safety

    std::array<char, 1024> _buffer;            // 수신 버퍼
    std::deque<std::string> _sendQueue;        // 송신 큐
};
