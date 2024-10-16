#include "../includes/ChnlTopicCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlTopicCmd::ChnlTopicCmd()
{
}

ChnlTopicCmd::~ChnlTopicCmd()
{
}

void	ChnlTopicCmd::execute(Client *client, const IRCMessage &message)
{
	std::string channelName = message.getParams()[0];
	std::string topic = message.getTrailing();
	std::string msg = ":" + client->getNickName() + " TOPIC " + channelName + " " + topic + "\r\n";
	if (Server::Singleton().getChannelByName(channelName)->getModes()->topicChannel == false)
	{
		if (Server::Singleton().getChannelByName(channelName)->isOperator(client) == false)
		{
			msg = "ERR_CHANOPRIVSNEEDED :You're not channel operator\r\n";
			Server::Singleton().sendMsg(client, msg);
			return ;
		}
		else
			Server::Singleton().getChannelByName(channelName)->getModes()->Topic = topic;
	}
	Server::Singleton().getChannelByName(channelName)->sendToAll(msg);
}

bool	ChnlTopicCmd::validate(const IRCMessage &msg)
{
	Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	std::string message;
	if (msg.getParams().size() < 1)
	{
		message = "ERR_NEEDMOREPARAMS TOPIC :Not enough parameters\r\n";
		Server::Singleton().sendMsg(client,message);
		return false;
	}
	std::string channelName = msg.getParams()[0];
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		message = "ERR_NOSUCHCHANNEL :No such channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->getClientByNickName(client->getNickName()) == 0)
	{
		message = "ERR_NOTONCHANNEL :You're not on that channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (msg.getTrailing().empty() && msg.getParams().size() == 1)
	{
		if (Server::Singleton().getChannelByName(channelName)->getModes()->Topic.empty())
		{
			message = "RPL_NOTOPIC :No topic is set\r\n";
			Server::Singleton().sendMsg(client, message);
			return false;
		}
		else
		{
			message = "RPL_TOPIC :" + Server::Singleton().getChannelByName(channelName)->getModes()->Topic + "\r\n";
			Server::Singleton().sendMsg(client, message);
			return false;
		}
	}
	execute(client, msg);
	return true;
}
