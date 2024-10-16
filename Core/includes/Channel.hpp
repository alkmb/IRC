#pragma once

#include "Client.hpp"

typedef struct mode
{
	bool		inviteOnly;
	bool		operOnly;
	bool		topicChannel;
	std::string	Topic;
	//bool		Channel;
	std::string	chanCreator;
} s_mode;

class	Channel
{
	private:
		std::deque<Client*>	_clients;
		std::deque<Client*>	_operators;
		std::string				_name;
		char					_channelPrefix;
		// CHANNEL MODES
		std::string				_key;
		int						_limit;
		s_mode					_modes;

	public:
		Channel				&operator+=(Client *cli);
		Channel				&operator-=(Client *cli);

		std::deque<Client*>	*getClientsFromChannel();
		Client*					getClientByNickName(std::string name);
		Client*					getClientByRealName(std::string name);
		struct pollfd			*getClientFd(Client* client);

		std::string				getChannelName();
		std::deque<Client*>	*getOperators();
		bool					isOperator(Client *client);
		std::string				&getChanCreator() const;
		const std::string		&getKey() const;

		//s_mode					getMode();
		int						getLimit() const;
		char					getChannelPrefix() const;

		void					sendToAll(const std::string &msg);
		bool					setName(const std::string &name);
		s_mode					*getModes();
	Channel();
	~Channel();

	private:
		bool					isFirstChannelChar(const char c) const;
		void					setChannelModes(Client *client);
};


/*
**	REVISAR LOS NUEVOS ATRIBUTOS QUE HE AGREGADO PARA GESTIONAR
	LOS MODOS, LIMITES Y PWD DE LOS CANALES

	POR SI SE NECESITARAN AGREGAR FUNCIONES NUEVAS
*/
