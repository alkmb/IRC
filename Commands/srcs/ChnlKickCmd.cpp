#include "../includes/ChnlKickCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlKickCmd::ChnlKickCmd()
{
}

ChnlKickCmd::~ChnlKickCmd()
{
}

void	ChnlKickCmd::execute(Client *client, IRCMessage const &message)
{
	Channel *channel = Server::Singleton().getChannelByName(message.getParams()[0]);
	std::string nickToKick = message.getParams()[1];
	std::string reason = message.getTrailing();
	std::string msg = ":" + client->getNickName() + " KICK " + channel->getChannelName() + " " + nickToKick + " :" + reason + "\r\n";
	Server::Singleton().sendMsgAll(msg);
	(*channel) -= channel->getClientByNickName(nickToKick);
}

bool	ChnlKickCmd::validate(IRCMessage const&msg)
{
	Client	*client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	std::string message;
	if (msg.getParams().size() < 3)
	{
		message = ":" + client->getNickName() + "ERR_NEEDMOREPARAMS KICK :Not enough parameters\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	std::string channelName = msg.getParams()[0];
	std::string nickToKick = msg.getParams()[1];
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		message = ":" + client->getNickName() + "ERR_NOSUCHCHANNEL :No such channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->isOperator(client) == false)
	{
		message = ":" + client->getNickName() + "ERR_CHANOPRIVSNEEDED :You're not channel operator\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getClientByNickName(nickToKick) == 0)
	{
		message = ":" + client->getNickName() + "ERR_NOSUCHNICK :No such nick\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	execute(client, msg);
	return true;
}
