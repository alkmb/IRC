#include "../includes/AuthNickCmd.hpp"

#include "../Headers.h"

AuthNickCmd::AuthNickCmd()
{
}

AuthNickCmd::~AuthNickCmd()
{
}

void NickCommand::execute(Client *client, IRCMessage &message)
{
	client->setNick(message._params[0]);
	Server::Singleton().sendMsg(client, message._command + message._params[0] + "\r\n");
}

bool NickCommand::validate(IRCMessage &message)
{

	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	if (message._params[0].empty())
	{
		Server::Singleton().sendMsg(client, "ERR_NONICKNAMEGIVEN :No nickname given\r\n");
		return false;
	}
	if (!message.check())
	{
		Server::Singleton().sendMsg(client, "ERR_ERRONEUSNICKNAME " + message._params[0] + " :Erroneous nickname\r\n");
		return false;
	}
	if (Server::Singleton().getClientByNickName(message._params[0]) != 0
		&& Server::Singleton().getClientByNickName(message._params[0]) != client)
	{
		Server::Singleton().sendMsg(client, "ERR_NICKNAMEINUSE " + message._params[0] + " :Nickname is already in use\r\n");
		return false;
	}
}
