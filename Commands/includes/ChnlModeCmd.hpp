#include "ICommand.hpp"
#include "../../Core/includes/Server.hpp"

#define MODES "[+/-] set/unset\n\ti - INVITE mode\
\n\tt - TOPIC of de channel\n\tk - KEY from for channel\
\n\to - OPERATOR user for channel\n\tl - LIMIT of users in channel"

class ChnlModeCmd : public ICommand
{
	public:
		ChnlModeCmd();
		~ChnlModeCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&message);

	private:
		void	parseModes(const std::string &modes, const std::vector<std::string> &params, size_t &paramIndex, Client *Client, Channel *channel);
		void	applyMode(char mode, bool adding, std::string &param, Client *client, Channel *channel);
		void	handleModeI(bool adding, Channel *channel);
		void	handleModeT(bool adding, Channel *channel, std::string &trailing);
		//void	handleModeK(bool adding, std::string &key, Channel *channel);
		//void	handleModeO(bool adding, Client *client, Channel *channel);
		//void	handleModeL(bool adding, const std::string &limitStr, Channel *channel);
};
