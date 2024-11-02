#include "../includes/AuthNickCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthNickCmd::AuthNickCmd()
{
}

AuthNickCmd::~AuthNickCmd()
{
}

void AuthNickCmd::execute(Client *client, IRCMessage const&message)
{
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	if (client->getNickName().empty())
		client->setNick(str);
	Server::Singleton().sendMsg(client, ":" + client->getNickName() + " NICK " + str + "\r\n");
	client->setNick(str);
	if (!client->isVerified() && client->getRealName() != "")
		client->setVerified();
}

bool AuthNickCmd::validate(IRCMessage const&message)
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
	if (str.empty())
	{
		Server::Singleton().sendMsg(client, ":Server 431 :No nickname given\r\n");
		return false;
	}
	if (false) //TO DO, implementar check de caracteres correctos
	{
		Server::Singleton().sendMsg(client, ":Server 432 " + str + " :Erroneous nickname\r\n");
		return false;
	}
	if (Server::Singleton().getClientByNickName(str) != 0
		&& Server::Singleton().getClientByNickName(str) != client)
	{
		Server::Singleton().sendMsg(client, ":Server 433 " + str + " :Nickname is already in use\r\n");
		return false;
	}

	execute(client, message);
	return true;
}
