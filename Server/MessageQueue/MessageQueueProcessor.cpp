#include "MessageQueueProcessor.h"


void MessageQueueProcessor::Start(MessageQueueHandler onMessage)
{
	_onMessage = std::move(onMessage);
}

void MessageQueueProcessor::Enqueue(std::shared_ptr<IInternalMessage> msg)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(std::move(msg));
}

void MessageQueueProcessor::Tick()
{
	std::queue<std::shared_ptr<IInternalMessage>> localQueue;

	// �ּ����� ��� �ð����� ��ü
	{
		std::lock_guard<std::mutex> lock(_mutex);
		std::swap(localQueue, _queue);
	}

	while (!localQueue.empty())
	{
		auto message = localQueue.front();
		if(_onMessage)
			_onMessage(message);

		localQueue.pop();
	}
}
