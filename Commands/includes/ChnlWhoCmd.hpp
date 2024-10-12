#pragma once

#include "ChannelCommand.hpp"

class	ChnlWhoCmd : public ChannelCommand
{
	private:
		struct pollfd	*_clFd;
		Client			*_client;
	public:
		ChnlWhoCmd();
		~ChnlWhoCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&msg);
};
