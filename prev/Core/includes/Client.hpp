#pragma once
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <csignal>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

class Client{
    private:
        std::string           _nick;
        bool                  _isAdmin;
        std::string           _real;
        struct pollfd         *_fd;
    public:
        struct pollfd   *getFd();
        std::string getNickName();
        std::string getRealName();
        bool    isAdmin();
        void    setFd(struct pollfd *fd);
        void    setNick(const std::string &name);
        void    setReal(const std::string &name);
        void    setAdmin(bool admin);
    Client();
    ~Client();
};
