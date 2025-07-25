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

    void StartSession();                       // ���� ���� (read ����)
    void Send(const std::string& msg);         // Ŭ���̾�Ʈ�� �޽��� ������
    void OnDisconnected();                     // ���� ���� ó��

    uint64_t GetSessionId() const { return _sessionId; }

private:
    void DoRead();                             // ���� read ó��
    void DoWrite(const std::string& msg);      // ���� write ó��
    void HandleCommand(const std::string& command); // ������ ��� ó��

private:
    uint64_t _sessionId;

    boost::asio::ip::tcp::socket _socket;                         // ����
    boost::asio::strand<boost::asio::io_context::executor_type> _strand; // strand for thread safety

    std::array<char, 1024> _buffer;            // ���� ����
    std::deque<std::string> _sendQueue;        // �۽� ť
};
