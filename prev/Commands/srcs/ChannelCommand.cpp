#include "../includes/ChannelCommand.hpp"

ChannelCommand::ChannelCommand()
{
}

ChannelCommand::~ChannelCommand()
{
}

bool	ChannelCommand::isOperator()
{
	return Server::Singleton()._channels[this->_channel].isOperator(this->_client);
}
