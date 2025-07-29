#pragma once
#include "../../../ECS/IComponent.h"
#include "../../../ECS/IDisposable.h"
#include "../../../Session/User/UserSession.h"

class ZoneUserComponent : public IComponent, public IDisposable
{
public:
	ZoneUserComponent();
	virtual ~ZoneUserComponent();
	void Dispose() override;

	void Tick();
	void Add(std::shared_ptr<UserSession> user);
	void Remove(uint64_t userUid);

private:
	std::mutex _mutex;
	std::unordered_map<uint64_t, std::weak_ptr<UserSession>> _users;
};

