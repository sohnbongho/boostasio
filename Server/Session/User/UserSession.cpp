#include "UserSession.h"
#include "GameCommandDispatcher.h"
#include "../Manager/UserSessionShardManager.h"
#include "../Accessor/PacketReceiver.h"
#include "../Accessor/PacketSender.h"

#include <iostream>

extern GameCommandDispatcher g_dispatcher;

UserSession::UserSession(uint64_t sessionId, boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& context)
	: _sessionId(sessionId),
	_socket(std::move(socket)),
	_strand(context.get_executor()),
	_receiver(std::make_shared<PacketReceiver>(_socket, _strand)),
	_sender(std::make_shared<PacketSender>(_socket, _strand))
{
	std::cout << "[Session] Constructed in context: " << &context << std::endl;
}
UserSession::~UserSession()
{
	std::cout << "[Session] Destory in context: " << std::endl;
}

void UserSession::StartSession()
{
	auto self = shared_from_this();

	_receiver->Start(
		[this, self](const Messages::MessageWrapper& msg) {
			HandleMessage(msg);
		},
		[this, self]() {
			OnDisconnected();
		});
}

void UserSession::Send(const std::string& msg)
{
	if(_sender)
		_sender->Send(msg);
}

void UserSession::Tick()
{
	std::cout << "[Session] Tick: " << _sessionId << std::endl;
}

void UserSession::OnDisconnected()
{
	std::cout << "[Session] Disconnected ID: " << _sessionId << std::endl;

	boost::system::error_code ec;
	_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	_socket.close(ec);  // 연결 종료

	UserSessionShardManager::Instance().RemoveSession(_sessionId);

	if (_receiver)
	{
		_receiver->Stop();  // ✅ Stop 시 람다 클리어 처리 필요
		_receiver = nullptr;
	}
	if (_sender)
	{
		_sender->Stop();
		_sender = nullptr;
	}	
}

void UserSession::HandleMessage(const Messages::MessageWrapper& msg)
{
	if (msg.has_connected_response())
	{
		std::cout << "[Proto] ConnectedResponse index: "
			<< msg.connected_response().index() << std::endl;
	}
	else if (msg.has_keep_alive_request())
	{
		std::cout << "[Proto] KeepAliveRequest received" << std::endl;
	}
}
