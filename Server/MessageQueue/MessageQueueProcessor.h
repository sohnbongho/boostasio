#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <queue>
#include "../Model/IInternalMessage.h"

class MessageQueueProcessor : public std::enable_shared_from_this<MessageQueueProcessor>
{
public:
	using MessageQueueHandler = std::function<void(std::shared_ptr<IInternalMessage> )>;

	MessageQueueProcessor() = default;

	~MessageQueueProcessor();

	void Start(MessageQueueHandler onMessage);

	void Enqueue(std::shared_ptr<IInternalMessage> msg);
	void Tick();

private:
	std::mutex _mutex;
	std::queue<std::shared_ptr<IInternalMessage>> _queue;
	MessageQueueHandler _onMessage;
};

