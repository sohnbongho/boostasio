#pragma once
#include "../Session/IoContextPool.h"
#include <vector>
#include <memory>

class TickSchedulerManager
{
public:
	TickSchedulerManager(IoContextPool& ioPool, int monsterThreadCount);
	void Start();	

private:	
	void StartZoneTickLoop();
	void ScheduleNextZoneTick(int shardId);

private:
	IoContextPool& _ioPool;
	std::vector<boost::asio::io_context*> _userContexts;	
	int _zoneThreadCount;
};
