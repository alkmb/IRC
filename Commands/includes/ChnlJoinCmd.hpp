#pragma once

#include "ChannelCommand.hpp"

class	ChnlJoinCmd : public ChannelCommand
{
	private:
		struct pollfd	*_clFd;
		Client			*_client;
	public:
		ChnlJoinCmd();
		~ChnlJoinCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&msg);
};
