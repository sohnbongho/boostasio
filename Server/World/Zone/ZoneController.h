#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "../User/ZoneUserManager.h"

class ZoneController : public std::enable_shared_from_this<ZoneController>
{
public:	
	ZoneController(int id, int mapId);
	~ZoneController();
	void Tick();
	void Load();
private:
	std::shared_ptr<ZoneUserManager> _userManager;
	int _id;
	int _mapId;

};

