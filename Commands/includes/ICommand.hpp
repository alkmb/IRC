#pragma once

#include "../../Core/includes/Channel.hpp"
#include "../../Core/includes/Client.hpp"
#include "../../Core/includes/IRCMessage.hpp"

class	ICommand
{
	public:
		ICommand() {};
		virtual ~ICommand() {};

		virtual void	execute(Client *client, IRCMessage const&message) = 0;
		virtual bool	validate(IRCMessage const& message) = 0;
};



