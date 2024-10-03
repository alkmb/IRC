#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class ircServer
{
    private:

    public:
      int _serverSocket;
      int _port;
      struct sockaddr_in serverAddress;
      ircServer();
      ircServer(const ircServer &obj);
      ircServer& operator=(const ircServer &obj);
      ~ircServer();
};

#endif
