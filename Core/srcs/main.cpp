#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <signal.h>

#include "../includes/IRCMessage.hpp"
#include "../includes/Server.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    unsigned int port = static_cast<unsigned int>(atoi(argv[1]));
    if (port > 65535)
    {
        std::cerr << "Error: Port is out of range for TCP/IP protocol." << std::endl;
        return 1;
    }

    if (Server::Singleton().initialize(argv[2], port) != 0)
    {
        std::cerr << "Server initialization failed" << std::endl;
        return 1;
    }

    Server::Singleton().serverLoop();

    return 0;
}