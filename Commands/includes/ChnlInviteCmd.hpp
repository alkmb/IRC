#include "ICommand.hpp"

class	ChnlInviteCmd : public ICommand
{
	public:
		ChnlInviteCmd();
		~ChnlInviteCmd();

		void	execute(Client *client,const IRCMessage &message);
		bool	validate(const IRCMessage &msg);
};
