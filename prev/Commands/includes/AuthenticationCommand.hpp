#pragma once

#include "ICommand.hpp"

class	AuthenticationCommand : public ICommand
{
	protected:
		bool		_isClientRegistered();

	public:
		virtual	~AuthenticationCommand();

		virtual void	execute() = 0;
		virtual bool	validate() = 0;
	// Comandos de autenticación: PASS, NICK, USER
};
