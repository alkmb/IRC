#include "../includes/AuthUserCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthUserCmd::AuthUserCmd()
{
}

AuthUserCmd::~AuthUserCmd()
{
}

void AuthUserCmd::execute(Client *client, IRCMessage const&message)
{
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	client->setReal(str);
	if (!client->isVerified() && client->getNickName() != "")
		client->setVerified();
	// //std::cout << "UserName: " << client->getRealName() << " | NickName: " << client->getNickName() << std::endl;
}

bool AuthUserCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	if (!client->correctPwd() && (!Server::Singleton().getPasswd().empty() || Server::Singleton().getPasswd() != ""))
	{
		Server::Singleton().sendMsg(client, ":Server 464 :Password incorrect\r\n");
		Server::Singleton() -= client;
		return false;
	}
	if ((message.getParams().size() != 3 && message.getTrailing().empty()) ||
		(message.getParams().size() == 3 && message.getTrailing().empty()) ||
		(message.getParams().size() != 3 && !message.getTrailing().empty()))
	{
		Server::Singleton().sendMsg(client, ":Server 461 USER: command needs more params\r\n");
		return false;
	}
	if (client->isVerified())
	{
		Server::Singleton().sendMsg(client, ":Server 462 :You may not reregister\r\n");
		return false;
	}
	execute(client, message);
	return true;
}
