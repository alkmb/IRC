#include "../includes/AuthPassCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthPassCmd::AuthPassCmd()
{
}

AuthPassCmd::~AuthPassCmd()
{
}

void AuthPassCmd::execute(Client *client, IRCMessage const&message)
{
	message.print();
	client->setPwd();
}

bool AuthPassCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	if (str.empty() && (!Server::Singleton().getPasswd().empty() || Server::Singleton().getPasswd() != ""))
	{
		Server::Singleton().sendMsg(client, ":Server 461 PASS :Not enough parameters\r\n");
		Server::Singleton() -= client;
		return false;
	}
	if (client->isVerified())
	{
		Server::Singleton().sendMsg(client, ":Server 462 :You may not reregister\r\n");
		return false;
	}
	if (str != Server::Singleton().getPasswd())
	{
		Server::Singleton().sendMsg(client, ":Server 464 :Password incorrect\r\n");
		Server::Singleton() -= client;
		return false;
	}
	execute(client, message);
	return true;
}
