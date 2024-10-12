#pragma once

#include "ICommand.hpp"

class	AuthenticationCommand : public ICommand
{
	protected:
		bool		_isClientRegistered();

	public:
		AuthenticationCommand() {};
		virtual ~AuthenticationCommand() {};

		virtual void	execute(Client *client, IRCMessage const&message) = 0;
		virtual bool	validate(IRCMessage const&message) = 0;
	// Comandos de autenticación: PASS, NICK, USER
};
