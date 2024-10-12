#pragma once

#include "Client.hpp"

struct mode
{
	bool		inviteOnly;
	bool		privateChannel;
	bool		secretChannel;
	bool		operOnly;
	bool		chanCreator;
};

class	Channel
{
	private:
		std::vector<Client*>	_clients;
		std::vector<Client*>	_operators;
		std::string				_name;
		int						_limit;
		std::string				*_key;

	public:
		Channel				&operator+=(Client *cli);
		Channel				&operator-=(Client *cli);

		std::vector<Client*>	*getClientsFromChannel();
		Client*					getClientByNickName(std::string name);
		Client*					getClientByRealName(std::string name);
		struct pollfd			*getClientFd(Client* client);

		std::string				getChannelName();
		std::vector<Client*>	*getOperators();

		void					sendToAll(const std::string &msg);
		void					setName(const std::string &name);
	Channel();
	~Channel();
};


/*
**	REVISAR LOS NUEVOS ATRIBUTOS QUE HE AGREGADO PARA GESTIONAR
	LOS MODOS, LIMITES Y PWD DE LOS CANALES

	POR SI SE NECESITARAN AGREGAR FUNCIONES NUEVAS
*/
