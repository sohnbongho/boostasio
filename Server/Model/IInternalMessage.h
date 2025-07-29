#pragma once
#include <iostream>

enum  InternalMessageType
{
	EnterRoomReq = 1001,
	EnterRoomRes = 1002,
};

class IInternalMessage
{
public:
	virtual ~IInternalMessage() = default; // ���� �Ҹ��� (�߿�)
	virtual InternalMessageType GetMessageType() const = 0; // ���� ���� �Լ�		
};


class EnterRoomReuqest : public IInternalMessage
{
public:
	virtual ~EnterRoomReuqest()
	{
		std::cout << "~EnterRoomReuqest" << std::endl;
	}

	InternalMessageType GetMessageType() const override
	{
		return InternalMessageType::EnterRoomReq;
	}
	EnterRoomReuqest(int zoneId, uint64_t userSessionId)
		: ZoneId(zoneId), UserSessionId(userSessionId)
	{
	}

	int ZoneId;
	uint64_t UserSessionId;
};

class EnterRoomResponse : public IInternalMessage
{
public:
	virtual ~EnterRoomResponse()
	{
		std::cout << "~EnterRoomResponse" << std::endl;
	}

	InternalMessageType GetMessageType() const override
	{
		return InternalMessageType::EnterRoomRes;
	}
	EnterRoomResponse(int zoneId, uint64_t userSessionId)
		: ZoneId(zoneId), UserSessionId(userSessionId)
	{
	}

	int ZoneId;
	uint64_t UserSessionId;
};
