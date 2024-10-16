#pragma once

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <deque>

class	Client
{
	private:
		std::string		_nick;
		std::string		_real;
		struct pollfd	*_fd;
		bool			_isOperator;
		bool			_isVerified;
	public:
		bool			operator==(const Client &n2);

		struct pollfd	*getFd();
		std::string		getNickName();
		std::string		getRealName();
		bool			isOperator();
		bool			isVerified();

		void			setVerified();
		void			setFd(struct pollfd *fd);
		void			setNick(const std::string &name);
		void			setReal(const std::string &name);
		void			setAdmin(bool admin);

	Client();
	~Client();
};
