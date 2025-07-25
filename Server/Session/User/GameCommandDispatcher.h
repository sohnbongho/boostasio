#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

class GameCommandDispatcher
{
public:
	using CommandHandler = std::function<void(const std::vector<std::string>& args)>;

	void RegisterCommand(const std::string& name, CommandHandler handler);
	void Dispatch(const std::string& commandLine);

private:
	std::unordered_map<std::string, CommandHandler> _handlers;
};