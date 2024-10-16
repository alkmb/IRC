#pragma once

#include "../../Core/includes/Server.hpp"
#include "MessageCommand.hpp"

class	MsgPrivmsgCmd : public MessageCommand
{
	public:
		MsgPrivmsgCmd();
		~MsgPrivmsgCmd();

		void execute(Client *client, IRCMessage const &message);
		bool validate(IRCMessage const &message);
};

/*
	PRIVMSG <msgtarget> <text>
		Se usa para enviar mensajes a otros usuarios, en principio no se usa para enviar mensajes a canales
		pero si se puede usar para enviar mensajes a servicios, bots, etc.

		<msgtarget> puede ser un usuario o un servicio, en principio no se usa para enviar mensajes a canales
		<text> es el mensaje que se quiere enviar

		Los errores que puede devolver son
			411 ERR_NORECIPIENT ":No recipient given (PRIVMSG)"
			412 ERR_NOTEXTTOSEND ":No text to send"
			413 ERR_NOTOPLEVEL ":No toplevel domain specified"
			414 ERR_WILDTOPLEVEL ":Wildcard in toplevel domain"
			415 ERR_BADMASK ":Bad Server/host mask"
*/
