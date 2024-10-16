#include "../includes/ChnlModeCmd.hpp"

ChnlModeCmd::ChnlModeCmd()
{
}

ChnlModeCmd::~ChnlModeCmd()
{
}

void	ChnlModeCmd::execute(Client *client, IRCMessage const &message)
{
	std::vector<std::string> params = message.getParams();
	std::string channelName = params[0];
	Channel *channel = Server::Singleton().getChannelByName(channelName);
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		std::cout << "ERR_NOSUCHCHANNEL " + client->getNickName() + " :No such channel" << std::endl;
		Server::Singleton().sendMsg(client, "ERR_NOSUCHCHANNEL " + client->getNickName() + " :No such channel\r\n");
		return;
	}
	if (params.size() == 1)
	{
		std::string modes = "+";
		if (channel->getModes()->inviteOnly) modes += "i";
		if (channel->getModes()->topicChannel) modes += "t";
		//if (channel->getModes()->secretChannel) modes += "s";
		//if (channel->getModes()->operOnly) modes += "o";
		//if (channel->getModes()->limit) modes += "l";
		std::cout << "RPL_CHANNELMODEIS " + client->getNickName() + " " + channelName + " " + modes << std::endl;
		Server::Singleton().sendMsg(client, "RPL_CHANNELMODEIS " + client->getNickName() + " " + channelName + " " + modes + "\r\n");	}
	if (!channel->isOperator(client))
	{
		std::cout << "ERR_CHANOPRIVSNEEDED " + client->getNickName() + " :You're not channel operator" << std::endl;
		Server::Singleton().sendMsg(client, "ERR_CHANOPRIVSNEEDED " + client->getNickName() + " :You're not channel operator\r\n");
		return;
	}
	std::string modes = params[1];
	size_t paramIndex = 2;
	parseModes(modes, params, paramIndex, client, channel);
	std::string modeMessage = ":" + client->getNickName() + " MODE " + channelName + " " + modes;
	std::cout << modeMessage << std::endl;
	for (size_t i = 2; i < params.size(); ++i)
		modeMessage += " " + params[i];
	modeMessage += "\r\n";
	channel->sendToAll(modeMessage.c_str());
}

bool	ChnlModeCmd::validate(IRCMessage const&msg)
{
	struct pollfd	*_clFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(_clFd);

	std::cout << "Params[0]: " << msg.getParams()[0] << std::endl;
	std::cout << "Params[1]: " << msg.getParams()[1] << std::endl;
	//if (msg.getParams().size() < 2)
	//{
	//	std::cout << "Params[0]: " << msg.getParams()[0] << std::endl;
	//	std::cout << "Params[1]: " << msg.getParams()[1] << std::endl;
	//	Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
	//	return false;
	//}
	if (msg.getParams().empty())
	{
		if (client)
			Server::Singleton().sendMsg(client, "461 " + client->getNickName() + " MODE :Not enough parameters\r\n");
		return false;
	}
	std::cout << "paso de aqui" << std::endl;
	execute(client, msg);
	//if (msg.getParams()[1][1] != '#') // Si es un modo de usuario
	//{
	//	std::string nick = msg.getParams()[1].substr(1);
	//	if (msg.getParams().size() < 3)
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
	//		return false;
	//	}
	//	if (nick.empty())
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NOSUCHNICK :No such nick\r\n");
	//		return false;
	//	}
	//	if (Server::Singleton().getClientByNickName(nick))
	//	{
	//		std::string modes = msg.getParams()[2];
	//		size_t paramIndex = 3;
	//	}
	//}
	//if (msg.getParams()[1][1] == '#') // Si es un modo de canal
	//{
	//	std::string channelName = msg.getParams()[1];
	//	if (msg.getParams().size() < 3)
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
	//		return false;
	//	}
	//	if (channelName.empty())
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NOSUCHCHANNEL :No such channel\r\n");
	//		return false;
	//	}
	//	if (Server::Singleton().getChannelByName(channelName))
	//	{
	//		std::string modes = msg.getParams()[2];
	//		size_t paramIndex = 3;

	//	}
	//}
	return true;
}

void	ChnlModeCmd::applyMode(char mode, bool adding, std::string &param, Client *client, Channel *channel)
{
	switch (mode)
	{
		case 'i':
			handleModeI(adding, channel);
			break;
		case 't':
			handleModeT(adding, channel, param);
			break;
		//case 'k':
		//	handleModeK(adding, param, channel);
		//	break;
		//case 'o':
		//	handleModeO(adding, client, channel);
		//	break;
		//case 'l':
		//	handleModeL(adding, param, channel);
		//	break;
		default:
			Server::Singleton().sendMsg(client, ":" + client->getNickName() +  "ERR_UNKNOWNMODE MODE :Unknown mode\r\n");
			break;

	}
}

void	ChnlModeCmd::parseModes(const std::string &modes, const std::vector<std::string> &params, size_t &paramIndex, Client *client, Channel *channel)
{
	bool	adding = true;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i] == '+')
			adding = true;
		else if (modes[i] == '-')
			adding = false;
		else
		{
			std::string param;
			std::cout << "modes: " << modes << std::endl;
			if (modes[i] == 'o' || modes[i] == 'k' || (modes[i] == 'l' && adding))
			{
				if (paramIndex >= params.size())
				{
					Server::Singleton().sendMsg(client, ":" + client->getNickName() + "461 " + client->getNickName() + " MODE :Not enough parameters\r\n");
					continue;
				}
			}
			applyMode(modes[i], adding, param, client, channel);
		}
	}
}

void	ChnlModeCmd::handleModeI(bool adding, Channel *channel)
{
	if (adding)
	{
		channel->getModes()->inviteOnly = true;
		Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
		std::string message =":" + client->getNickName() + "MODE " + channel->getChannelName() + " +i\r\n";
		Server::Singleton().sendMsg(client, message);
	}
	else
		channel->getModes()->inviteOnly = false;
}

void	ChnlModeCmd::handleModeT(bool adding, Channel *channel, std::string &trailing)
{
	Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	if (adding)
	{
		channel->getModes()->topicChannel = true;
		std::string message = "MODE " + channel->getChannelName() + " +t :" + trailing + "\r\n";
		std::cout << "message: " << message << std::endl;
		Server::Singleton().sendMsg(client,message);
	}
	else
	{
		channel->getModes()->topicChannel = false;
		std::string message = "MODE " + channel->getChannelName() + " -t\r\n";
		Server::Singleton().sendMsg(client,message);
	}
}
