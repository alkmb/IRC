#pragma once

#include "ICommand.hpp"

class	ChannelCommand : public ICommand
{
	public:
		ChannelCommand() {};
		virtual	~ChannelCommand() {};

		virtual void	execute(Client *client, IRCMessage const&message) = 0;
		virtual bool	validate(IRCMessage const&message) = 0;
};
