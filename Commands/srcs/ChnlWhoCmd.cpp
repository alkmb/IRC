#include "../includes/ChnlWhoCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlWhoCmd::ChnlWhoCmd()
{

}

ChnlWhoCmd::~ChnlWhoCmd()
{
	
}

void	ChnlWhoCmd::execute(Client *client, IRCMessage const&message)
{
	//send message to all clients in the channel
	//send message to the client that joined the channel
}

bool	ChnlWhoCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	_clFd = Server::Singleton().getCurrentFd();
	_client = Server::Singleton().getClientByFd(_clFd);
	Channel *chan = Server::Singleton().getChannelByName(msg.getParams()[0]);
	for	(int i = 0; i < (*chan->getClientsFromChannel()).size(); i++)
	{
		Client *to = (*chan->getClientsFromChannel())[i];
		if  (i != (*chan->getClientsFromChannel()).size() - 1)
			chan->sendToAll(":Server 352 " + to->getNickName() + " #" + chan->getChannelName());
		else
			chan->sendToAll(":Server 351 " + to->getNickName() + " #" + chan->getChannelName());
	}
	execute(_client, msg);
	return true;
}
