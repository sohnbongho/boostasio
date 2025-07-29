#include <iostream>
#include "./Session/IoContextPool.h"
#include "./Session/TcpAcceptor.h"

#include "./World/Monster/MonsterCommandQueue.h"
#include "./World/Monster/MonsterObject.h"
#include "./World/Monster/MonsterThreadPool.h"
#include "./Session/User/GameCommandDispatcher.h"

#include "WorldServer.h"
#include "./Thread/TickSchedulerManager.h"
#include "./World/Zone/ZoneManager.h"


MonsterThreadPool g_monsterPool(2);
GameCommandDispatcher g_dispatcher;

void WorldServer::Run()
{
	try
	{
		auto port = 12345;
		auto ioContextCount = 1;
		auto zoneThreadCount = ZoneManager::Instance().ShardCount;

		ZoneManager::Instance().Load();		

		IoContextPool userPool(ioContextCount);
		TcpAcceptor userServer(userPool, port);
		TickSchedulerManager tickScheduler(userPool, zoneThreadCount);

		tickScheduler.Start();

		std::cout << "[Main] Starting IoContext Pool\n";
		userPool.Run();
		std::cout << "[Main] End IoContext Pool\n";

		g_monsterPool.Stop();
		userPool.Stop();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
