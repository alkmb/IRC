#include "ICommand.hpp"
#include "../../Core/includes/Server.hpp"

#define MODES "\n\t[+/-] set/unset\n\ti - INVITE mode\
\n\tt - TOPIC of de channel\n\tk - KEY for channel\
\n\to - OPERATOR user for channel\n\tl - LIMIT of users in channel"

class ChnlModeCmd : public ICommand
{
	private:
		std::string appliedModes;
	public:
		ChnlModeCmd();
		~ChnlModeCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&message);

	private:
		bool	parseModes(const std::string &modes, const std::vector<std::string> &params,\
							size_t &paramIndex, Client *Client,\
							std::string &modesToApply, std::vector<std::string> &paramsToApply);
		void	applyModes(Channel *channel, const std::string &modesToApply, const std::vector<std::string> &paramsToApply, Client *client);
		//char	applyMode(char mode, bool adding, s&param, Client *client, Channel *channel);
		bool	isValidMode(char mode);
		bool	modeRequiresParam(char mode, bool adding);
};
