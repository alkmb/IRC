#include "ICommand.hpp"

class	ChnlTopicCmd : public ICommand
{
	public:
		ChnlTopicCmd();
		~ChnlTopicCmd();

		void	execute(Client *client,const IRCMessage &message);
		bool	validate(const IRCMessage &msg);
};
