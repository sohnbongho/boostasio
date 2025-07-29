#include "ZoneController.h"
#include "Component/ZoneUserComponent.h"


ZoneController::ZoneController(int id, int mapId)
	:_id(id),
	_mapId(mapId),
	_ecsEntity(std::make_shared<Entity>()),
	_messageQueueProcessor(std::make_shared<MessageQueueProcessor>())
{
	std::cout << "[ZoneController] ctor: mapId:" << _mapId << " id:" << _id << std::endl;
	InitEcs();
}
ZoneController::~ZoneController()
{
	std::cout << "[ZoneController] Destory in context: " << std::endl;
	_ecsEntity = nullptr;
}

void ZoneController::Tick()
{
	std::cout << "[ZoneController] Tick() id: " << _id << std::endl;
	auto userComponent = _ecsEntity->GetComponent<ZoneUserComponent>();
	if (userComponent)
		userComponent->Tick();

	if (_messageQueueProcessor)
		_messageQueueProcessor->Tick();

}

void ZoneController::InitEcs()
{
	_ecsEntity->AddComponent<ZoneUserComponent>(std::make_shared<ZoneUserComponent>());
}

void ZoneController::AddUser(std::shared_ptr<UserSession> user)
{
	auto userComponent = _ecsEntity->GetComponent<ZoneUserComponent>();
	if (userComponent == nullptr)
		return;

	userComponent->Add(user);
}
void ZoneController::Load() 
{

}