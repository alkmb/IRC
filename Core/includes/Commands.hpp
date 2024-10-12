#pragma once

#include <map>

class Commands
{
	private:
		std::map<std::string, std::vector<std::string> params>	_commands;

	public:
		Commands();
		~Commands();
};

Commands::Commands(/* args */)
{
}

Commands::~Commands()
{
}
