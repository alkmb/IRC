#pragma once

#include "../../Headers.h"

class	ICommand
{
	public:
		virtual	~ICommand();

		virtual void	execute() = 0;
		virtual bool	validate(IRCMessage *message) = 0;
};

ICommand::~ICommand()
{
}

class ChannelCommand : public ICommand
{
    // Comandos de canal: JOIN, PART, MODE, TOPIC, INVITE, KICK
};

class MessagingCommand : public ICommand
{
    // Comandos de mensajería: PRIVMSG, NOTICE
};

//	el comando QUIT alomejor no quieres hacer clase derivada pero estaría bien
//	porqué se derivaría de esta clase que sería la clase base de otros comandos como PONG

class ConnectionCommand : public ICommand
{
    // Comandos de conexión: QUIT
};



