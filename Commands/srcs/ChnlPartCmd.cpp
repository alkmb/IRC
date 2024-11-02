#include "../includes/ChnlPartCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlPartCmd::ChnlPartCmd()
{

}

ChnlPartCmd::~ChnlPartCmd()
{
}

void	ChnlPartCmd::execute(Client *client, IRCMessage const&message)
{
	Server::Singleton().sendMsg(client,":" + client->getNickName() + " PART " + message.getParams()[0] + "\r\n");
}

bool	ChnlPartCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	_clFd = Server::Singleton().getCurrentFd();
	_client = Server::Singleton().getClientByFd(_clFd);
	std::string str = msg.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	std::string response = "";
	Channel *chan = Server::Singleton().getChannelByName(str);
	if (msg.getParams().size() == 0 || msg.getParams()[0].empty())
	{
		response += ": Server 461 " + _client->getNickName() + " " + msg.getCommand() + " :Not enough parameters\r\n";
		Server::Singleton().sendMsg(_client, response);
		return false;
	}
	if (chan == 0)
	{
		response += ": Server 403 " + _client->getNickName() + " " + str + " :No such channel\r\n";
		Server::Singleton().sendMsg(_client, response);
		return false;
	}
	if (chan->getClientByNickName(_client->getNickName()) == 0)
	{
		response += ": Server 442 " + _client->getNickName() + " " + str + " :You're not on that channel\r\n";
		Server::Singleton().sendMsg(_client, response);
		return false;
	}
	execute(_client, msg);
	return true;
}
