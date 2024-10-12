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
	client->setVerified();
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
		Server::Singleton().sendMsg(client, "ERR_NEEDMOREPARAMS PASS :Not enough parameters\r\n");
		Server::Singleton() -= client;
		return false;
	}
	if (client->isVerified())
	{
		Server::Singleton().sendMsg(client, "ERR_ALREADYREGISTERED :You may not reregister\r\n");
		return false;
	}
	if (str != Server::Singleton().getPasswd())
	{
		Server::Singleton().sendMsg(client, "ERR_PASSWDMISMATCH :Wrong Password\r\n");
		Server::Singleton() -= client;
		return false;
	}
	execute(client, message);
	return true;
}
