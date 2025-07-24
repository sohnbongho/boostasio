#pragma once
#include <memory>
#include "../Session/UserSession.h"

class UserWorkerThread
{
public:
	void AddSession(std::shared_ptr<UserSession> session);
	void Start();
	void Stop();

private:
	std::atomic<bool> _running{ true };
	std::thread _thread;
	std::mutex _mutex;
	std::vector<std::shared_ptr<UserSession>> _sessions;
};

