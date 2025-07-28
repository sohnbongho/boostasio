#include "ZoneController.h"

ZoneController::ZoneController(int id, int mapId)
	:_userManager(std::make_shared<ZoneUserManager>()), _id(id), _mapId(mapId)
{
	std::cout << "[ZoneController] ctor: mapId:" << _mapId << " id:" << _id << std::endl;
}
ZoneController::~ZoneController()
{
	std::cout << "[ZoneController] Destory in context: " << std::endl;
	_userManager = nullptr;
}

void ZoneController::Tick()
{
	std::cout << "[ZoneController] Tick() id: " << _id << std::endl;
	if(_userManager)
		_userManager->Tick();
}

void ZoneController::Load()
{

}
