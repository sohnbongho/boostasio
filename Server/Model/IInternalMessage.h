#pragma once


enum  InternalMessageType
{
	EnterRoomReq = 1,
};

class IInternalMessage
{
public:
	virtual ~IInternalMessage() = default; // ���� �Ҹ��� (�߿�)
	virtual InternalMessageType GetMessageType() const = 0; // ���� ���� �Լ�	
	void SetOnProcessed(std::function<void()> handler)
	{
		_onProcessed = std::move(handler);
	}

	void OnProcessed()
	{
		if (_onProcessed)
			_onProcessed();
	}
protected:
	std::function<void()> _onProcessed;
};


class EnterRoomReuqest : public IInternalMessage
{
public:
	InternalMessageType GetMessageType() const override
	{
		return InternalMessageType::EnterRoomReq;
	}
	EnterRoomReuqest(int zoneId, uint64_t userSeq)
		: ZoneId(zoneId), UserSeq(userSeq)
	{
	}

	int ZoneId;
	uint64_t UserSeq;
};
