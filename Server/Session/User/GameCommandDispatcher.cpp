#include "GameCommandDispatcher.h"

void GameCommandDispatcher::RegisterCommand(const std::string& name, CommandHandler handler)
{
    _handlers[name] = std::move(handler);
}

void GameCommandDispatcher::Dispatch(const std::string& commandLine)
{
    std::istringstream iss(commandLine);
    std::string cmd;
    iss >> cmd;

    if (cmd.empty()) return;

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg)
        args.push_back(arg);

    auto it = _handlers.find(cmd);
    if (it != _handlers.end())
    {
        it->second(args);
    }
    else
    {
        std::cout << "[Dispatcher] Unknown command: " << cmd << std::endl;
    }
}