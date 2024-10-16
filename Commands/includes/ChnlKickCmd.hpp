#pragma once

#include "ICommand.hpp"

class	ChnlKickCmd : public ICommand
{
	public:
		ChnlKickCmd();
		~ChnlKickCmd();

		void	execute(Client *client,const IRCMessage &message);
		bool	validate(const IRCMessage &msg);
};
