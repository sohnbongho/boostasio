#include "ZoneController.h"

ZoneController::ZoneController(int id)
	:_userManager(std::make_shared<UserManager>()), _id(id)
{
	std::cout << "[ZoneController] ctor: " << _id << std::endl;
}
ZoneController::~ZoneController()
{
	std::cout << "[ZoneController] Destory in context: " << std::endl;
	_userManager = nullptr;
}

void ZoneController::Tick()
{
	std::cout << "[ZoneController] Tick() id: " << _id << std::endl;

}
