#include <iostream>
#include "./Session/IoContextPool.h"
#include "./Monster/MonsterCommandQueue.h"
#include "./Monster/MonsterObject.h"
#include "./Monster/MonsterThreadPool.h"
#include "./Command/GameCommandDispatcher.h"
#include "Session/AsyncTcpServer.h"

MonsterThreadPool g_monsterPool(2);
GameCommandDispatcher g_dispatcher;

int main()
{
    try
    {
        IoContextPool userPool(4);
        AsyncTcpServer userServer(userPool, 12345); // Placeholder

        // 몬스터 등록
        g_monsterPool.AddMonster(std::make_shared<MonsterObject>(1, "Goblin"));
        g_monsterPool.AddMonster(std::make_shared<MonsterObject>(2, "Orc"));

        // Dispatcher 등록 예시
        g_dispatcher.RegisterCommand("CHAT", [](const std::vector<std::string>& args) {
            for (const auto& arg : args)
                std::cout << arg << " ";
            std::cout << std::endl;
            });

        g_monsterPool.Start();
        userPool.Run();

        g_monsterPool.Stop();
        userPool.Stop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}