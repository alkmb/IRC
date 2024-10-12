#pragma once

# include "AuthenticationCommand.hpp"

class	AuthNickCmd : public AuthenticationCommand
{
	public:
		AuthNickCmd();
		~AuthNickCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&message);
};

/*
	Solo puede tener 1 parametro en principio, revisar, los errores que devuelve el servidor
	pueden ser:
		431 ERR_NONICKNAMEGIVEN ":No nickname given"
		432 ERR_ERRONEUSNICKNAME "<nick> :Erroneous nickname"
		433 ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"
		437 ERR_UNAVAILRESOURCE "<nick/channel> :Nick/channel is temporarily unavailable"
		436 ERR_NICKCOLLISION "<nick> :Nickname collision KILL from <user>@<host>"
		484 ERR_RESTRICTED ":Your connection is restricted!"
*/
