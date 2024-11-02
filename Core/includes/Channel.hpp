#pragma once

#include "Client.hpp"
#include "../srcs/Get.cpp"
#include <list>

typedef struct mode
{
	bool		inviteOnly;
	bool		topicLock;
	std::string	Topic;
	std::string	chanCreator;
} s_mode;

class	Channel
{
	private:
		std::list<Client*>	_clients;
		std::list<Client*>	_operators;
		std::string			_name;
		char				_channelPrefix;
		// CHANNEL MODES
		std::string			_key;
		int					_limit;
		s_mode				_modes;
		std::deque<std::string> _inviteList;

	public:
		Channel				&operator+=(Client *cli);
		Channel				&operator-=(Client *cli);

		std::list<Client*>	*getClientsFromChannel();
		Client*				getClientByNickName(std::string name);
		Client*				getClientByRealName(std::string name);
		struct pollfd		*getClientFd(Client* client);

		std::string			getChannelName();
		std::list<Client*>	*getOperators();
		std::string			&getChanCreator() const;

		char				getChannelPrefix() const;

		void				sendMsgExcept(Client *c, const std::string &msg);
		void				sendToAll(const std::string &msg);
		bool				setName(const std::string &name);

		s_mode				*getModes();

		void				setKey(const std::string &key);
		void				removeKey();
		const std::string	&getKey() const;

		void				setLimit(int limit);
		void				removeLimit();
		int					getLimit() const;

		void				setInviteOnly(bool inviteOnly);
		bool				isInviteOnly()const;

		void				setTopicLock(bool topicLock);
		bool				isTopicLocked() const;
		const std::string	&gettopicLock() const;

		void				addOperator(Client *client);
		void				removeOperator(Client *client);
		bool				isOperator(Client *client);

		void				addInvite(std::string &nickname);
		bool				isInvited(const std::string &nickname) const;

	Channel();
	~Channel();

	private:
		bool				isFirstChannelChar(const char c) const;
};


/*
**	REVISAR LOS NUEVOS ATRIBUTOS QUE HE AGREGADO PARA GESTIONAR
	LOS MODOS, LIMITES Y PWD DE LOS CANALES

	POR SI SE NECESITARAN AGREGAR FUNCIONES NUEVAS
*/
