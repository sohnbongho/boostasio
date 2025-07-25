#include <iostream>
#include "./Session/IoContextPool.h"
#include "./Session/TcpAcceptor.h"

#include "./World/Monster/MonsterCommandQueue.h"
#include "./World/Monster/MonsterObject.h"
#include "./World/Monster/MonsterThreadPool.h"
#include "./Session/User/GameCommandDispatcher.h"

#include "WorldServer.h"


MonsterThreadPool g_monsterPool(2);
GameCommandDispatcher g_dispatcher;

void WorldServer::Run()
{
    try
    {
        IoContextPool userPool(2);
        TcpAcceptor userServer(userPool, 12345); // 반드시 Accept()를 시작해야 함

        /*g_monsterPool.AddMonster(std::make_shared<MonsterObject>(1, "Goblin"));
        g_monsterPool.AddMonster(std::make_shared<MonsterObject>(2, "Orc"));

        g_dispatcher.RegisterCommand("CHAT", [](const std::vector<std::string>& args) {
            for (const auto& arg : args)
                std::cout << arg << " ";
            std::cout << std::endl;
            });

        g_monsterPool.Start();*/

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
