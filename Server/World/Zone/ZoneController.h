#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "../../ECS/Entity.h"
#include "../../Session/User/UserSession.h"

class ZoneController : public std::enable_shared_from_this<ZoneController>
{
public:	
	ZoneController(int id, int mapId);
	~ZoneController();
	
	void Tick();	
	void AddUser(std::shared_ptr<UserSession> user);
	void Load();
private:
	void InitEcs();

private:	
	int _id;
	int _mapId;
	std::shared_ptr<Entity> _ecsEntity;
	std::shared_ptr<MessageQueueProcessor> _messageQueueProcessor;

};

