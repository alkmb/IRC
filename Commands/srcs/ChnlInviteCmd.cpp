#include "../includes/ChnlInviteCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlInviteCmd::ChnlInviteCmd()
{
}

ChnlInviteCmd::~ChnlInviteCmd()
{
}

void	ChnlInviteCmd::execute(Client *client, IRCMessage const &message)
{
	std::string nickToInvite = message.getParams()[0];
	std::string channelName = message.getParams()[1];
	std::string msg;
	if (Server::Singleton().getChannelByName(channelName)->getModes()->inviteOnly == true)
	{
		if (Server::Singleton().getChannelByName(channelName)->isOperator(client) == false)
		{
			msg =  ": " + client->getNickName() + " 482 " + channelName + " :You're not channel operator\r\n";
			Server::Singleton().sendMsg(client, msg);
			return;
		}
	}
	Server::Singleton().getChannelByName(channelName)->addInvite(nickToInvite);
	msg = ":" + client->getNickName() + " INVITE " + nickToInvite + " " + channelName + "\r\n";
	Server::Singleton().getChannelByName(channelName)->sendToAll(msg);
}

bool	ChnlInviteCmd::validate(const IRCMessage &msg)
{
	Client	*client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	std::string message;
	if (msg.getParams().size() != 2)
	{
		message = ": " + client->getNickName() + " 461 INVITE :Not enough parameters\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	std::string nickToInvite = msg.getParams()[0];
	if (Server::Singleton().getClientByNickName(nickToInvite) == 0)
	{
		message = ": " + client->getNickName() + " 401 :No such nick/channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	std::string channelName = msg.getParams()[1];
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		message = ": " + client->getNickName() + " 403 :No such channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->getClientByNickName(client->getNickName()) == 0)
	{
		message = ": " + client->getNickName() +" 442 :You're not on that channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->getClientByNickName(nickToInvite) != 0)
	{
		message = ": " + client->getNickName() + " 443 :User is already on channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	execute(client, msg);
	return true;
}
