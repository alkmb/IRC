#pragma once

#include "AuthenticationCommand.hpp"

class	AuthUserCmd : public AuthenticationCommand
{
		public:
		AuthUserCmd();
		~AuthUserCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&message);
	// Comando USER
};

/*
USER <user> <mode> <unused> :<realname>
	Se usa para especificar un nuevo usuario a la hora de conectarse al servidor

	<user> será el nombre de usuario
	<mode> se usa para especificar el modo de usuario, en principio no se usa, en
		el caso de que si, se usa 0 y 8 que es para setear el modo inivsible del usuario
	<unused> suele ser un '*' en la mayoría de los casos, se ignora
	<realname> es el nombre real del usuario, suele ir precedido de : así que en el
		IRCMessage se debería quedar guardado en el parámetro _trailing

	Los errores que puede devolver son
		461 ERR_NEEDMOREPARAMS "PASS :Not enough parameters"
		462 ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"
*/
