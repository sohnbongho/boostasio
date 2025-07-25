#include <iostream>
#include "./Session/IoContextPool.h"
#include "Session/AsyncTcpServer.h"

#include "./Monster/MonsterCommandQueue.h"
#include "./Monster/MonsterObject.h"
#include "./Monster/MonsterThreadPool.h"
#include "./Command/GameCommandDispatcher.h"

MonsterThreadPool g_monsterPool(2);
GameCommandDispatcher g_dispatcher;

int main()
{
    try
    {
        IoContextPool userPool(4);
        AsyncTcpServer userServer(userPool, 34567); // 반드시 Accept()를 시작해야 함

        /*g_monsterPool.AddMonster(std::make_shared<MonsterObject>(1, "Goblin"));
        g_monsterPool.AddMonster(std::make_shared<MonsterObject>(2, "Orc"));

        g_dispatcher.RegisterCommand("CHAT", [](const std::vector<std::string>& args) {
            for (const auto& arg : args)
                std::cout << arg << " ";
            std::cout << std::endl;
            });

        g_monsterPool.Start();*/

        std::cout << "[Main] Starting pool...\n";
        userPool.Run(); // 여기에 block
        std::cout << "[Main] Exiting...\n";

        g_monsterPool.Stop();
        userPool.Stop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
