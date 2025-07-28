#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "../User/UserManager.h"

class ZoneController : public std::enable_shared_from_this<ZoneController>
{
public:	
	ZoneController(int id);
	~ZoneController();
	void Tick();
private:
	std::shared_ptr<UserManager> _userManager;
	int _id;
};

