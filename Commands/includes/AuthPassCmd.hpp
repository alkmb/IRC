#pragma once

# include "AuthenticationCommand.hpp"

class	AuthPassCmd : public AuthenticationCommand
{
	public:
		AuthPassCmd();
		~AuthPassCmd();

		void	execute(Client *client, IRCMessage const&message);
		bool	validate(IRCMessage const&message); // Si la cantidad de Msg.params.size() > 1 entonces no es valido porque no puede tener espacios
};

/*
	DEBE tener entre 1 y 23 caracteres
	PUEDE contener cualquier carácter imprimible ASCII, excepto el espacio.
		es decir que puede contener caracteres de A-Z, a-z, 0-0 y
		! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _  { | } ~`.

	Los errores que puede devolver son
		461 ERR_NEEDMOREPARAMS "PASS :Not enough parameters"
		462 ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"
*/
