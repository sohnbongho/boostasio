﻿#include "UserSession.h"
#include "GameCommandDispatcher.h"
#include "../Manager/UserSessionShardManager.h"

#include <iostream>
#include <memory>
#include "../../World/ZoneManager/ZoneControllerManager.h"
#include "System/UserMessageDispatcher.h"
#include "Component/UserZoneInfoComponent.h"

extern GameCommandDispatcher g_dispatcher;

UserSession::UserSession(uint64_t sessionId, boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& context)
	: _sessionId(sessionId),
	_socket(std::move(socket)),
	_strand(context.get_executor()),
	_receiver(std::make_shared<PacketReceiver>(_socket, _strand)),
	_sender(std::make_shared<PacketSender>(_socket, _strand)),
	_messageQueueProcessor(std::make_shared<MessageQueueProcessor>()),
	_ecsEntity(std::make_shared<Entity>())
{
	// TODO: 임의로 처리
	_userUId = sessionId;

	std::cout << "[Session] Constructed in context: " << &context << std::endl;
	InitEcs();

	_messageQueueProcessor->Start(
		[this](std::shared_ptr<IInternalMessage>  msg) {
			OnRecvHandleMessage(msg);
		});
}
UserSession::~UserSession()
{
	std::cout << "[Session] Destory in context: " << std::endl;
	_receiver = nullptr;
	_sender = nullptr;
	_messageQueueProcessor = nullptr;
	if (_ecsEntity)
	{
		_ecsEntity->Dispose();
		_ecsEntity = nullptr;
	}
}

void UserSession::InitEcs()
{
	_ecsEntity->AddComponent<UserZoneInfoComponent>(std::make_shared<UserZoneInfoComponent>());

	_ecsEntity->AddSystem<UserMessageDispatcher>(std::make_shared<UserMessageDispatcher>());
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
	if (_sender)
		_sender->Send(msg);
}

void UserSession::Tick()
{
	std::cout << "[Session] Tick: " << _sessionId << std::endl;
	if (_messageQueueProcessor)
		_messageQueueProcessor->Tick();
}

void UserSession::OnDisconnected()
{
	std::cout << "[Session] Disconnected ID: " << _sessionId << std::endl;

	boost::system::error_code ec;
	_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	_socket.close(ec);  // 연결 종료	

	if (_receiver)
	{
		_receiver->Stop();
		_receiver = nullptr;
	}
	if (_sender)
	{
		_sender->Stop();
		_sender = nullptr;
	}
	if (_messageQueueProcessor)
	{
		_messageQueueProcessor = nullptr;
	}
	UserSessionShardManager::Instance().Remove(_sessionId);
}

void UserSession::HandleMessage(const Messages::MessageWrapper& msg)
{
	if (msg.has_connected_response())
	{
		std::cout << "[Proto] ConnectedResponse index: "
			<< msg.connected_response().index() << std::endl;

		auto zoneId = 1;
		auto sessionId = GetSessionId();

		std::shared_ptr<EnterRoomReuqest> message = std::make_shared< EnterRoomReuqest>(zoneId, sessionId);
		ZoneControllerManager::Instance().EnqueueMessage(message);
	}
	else if (msg.has_keep_alive_request())
	{
		std::cout << "[Proto] KeepAliveRequest received" << std::endl;
	}
}


void UserSession::EnqueueMessage(std::shared_ptr<IInternalMessage> msg)
{
	if (_messageQueueProcessor)
		_messageQueueProcessor->Enqueue(msg);
}

void UserSession::OnRecvHandleMessage(std::shared_ptr<IInternalMessage> message)
{
	std::cout << "OnRecvHandleMessage type:" << message->GetMessageType() << std::endl;

	auto userDispatcher = _ecsEntity->GetSystem<UserMessageDispatcher>();
	if (userDispatcher == nullptr)
		return;

	userDispatcher->OnRecvHandleMessage(_ecsEntity, message);

}