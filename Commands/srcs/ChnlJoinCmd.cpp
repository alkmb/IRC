#include "../includes/ChnlJoinCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlJoinCmd::ChnlJoinCmd()
{

}

ChnlJoinCmd::~ChnlJoinCmd()
{
}

void	ChnlJoinCmd::execute(Client *client, IRCMessage const&message)
{
	//send message to all clients in the channel
	//send message to the client that joined the channel
	Server::Singleton().sendMsg(_client, ":"+ _client->getNickName() + " JOIN #" + message.getParams()[0] + "\r\n");
}

bool	ChnlJoinCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	_clFd = Server::Singleton().getCurrentFd();
	_client = Server::Singleton().getClientByFd(_clFd);
	if (!_client->isVerified() /*|| (_client->isVerified() && (_client->getNickName().empty() || _client->getRealName().empty()))*/)
	{
		Server::Singleton().sendMsg(_client, "ERR_NOTREGISTERED :You have not registered\r\n");
		return false;
	}
	if (msg.getParams().size() < 1)
	{
		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS JOIN :Not enough parameters\r\n");
		return false;
	}
	if (Server::Singleton().getChannelByName(msg.getParams()[0]) == 0)
	{
		Server::Singleton() += msg.getParams()[0];
	}
	if (true) //check channel permissions;
	{
		//en caso de que se mueva el cliente, lo quitamos del channel en el que este y luego lo anadimos al nuevo
		Channel *current = Server::Singleton().getChannelByClient(_client);
		if (current != 0)
			*current -= _client;
		*Server::Singleton().getChannelByName(msg.getParams()[0]) += _client;
	}
	else
	{
		Server::Singleton().sendMsg(_client, "ERR_RESTRICTED :Your connection is restricted!\r\n");
		return false;
	}
	execute(_client, msg);
	return true;
}
