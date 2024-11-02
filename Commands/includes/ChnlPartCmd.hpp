#pragma once

#include "ChannelCommand.hpp"

class	ChnlPartCmd : public ChannelCommand
{
	private:
		struct pollfd	*_clFd;
		Client			*_client;
	public:
		ChnlPartCmd();
		~ChnlPartCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&msg);
};
