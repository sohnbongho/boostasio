#include "UserSession.h"
#include "GameCommandDispatcher.h"
#include <iostream>
#include "UserSessionManager.h"

extern GameCommandDispatcher g_dispatcher;

UserSession::UserSession(uint64_t sessionId, boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& context)
	: _socket(std::move(socket)), _buffer{}, _sessionId(sessionId), _strand(context.get_executor())
{
	std::cout << "[Session] Constructed in context: " << &context << std::endl;
}

void UserSession::StartSession()
{
	DoRead();
}

void UserSession::DoRead()
{
	auto self = shared_from_this();
	_socket.async_read_some(
		boost::asio::buffer(_buffer),
		boost::asio::bind_executor(_strand,
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::string received(_buffer.data(), length);
					std::cout << "[Recv] " << received << std::endl;
					HandleCommand(received);
					DoRead();
				}
				else
				{
					OnDisconnected();
				}
			}));
}

void UserSession::OnDisconnected()
{
	std::cout << "[Session] Disconnected ID: " << _sessionId << std::endl;
	UserSessionManager::Instance().RemoveSession(_sessionId);
}

void UserSession::Send(const std::string& msg)
{
	auto self = shared_from_this();
	boost::asio::post(_strand, [this, self, msg]()
		{
			bool writing = !_sendQueue.empty();
			_sendQueue.push_back(msg);
			if (!writing)
				DoWrite(_sendQueue.front());
		});
}


void UserSession::DoWrite(const std::string& msg)
{
	auto self = shared_from_this();
	boost::asio::async_write(_socket, boost::asio::buffer(msg),
		boost::asio::bind_executor(_strand,
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					_sendQueue.pop_front();
					if (!_sendQueue.empty())
						DoWrite(_sendQueue.front());
				}
			}));
}

void UserSession::HandleCommand(const std::string& command)
{
	g_dispatcher.Dispatch(command);
}