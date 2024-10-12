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

    Server::Singleton().initialize(argv[2], static_cast<unsigned short>(atoi(argv[1])));
    Server::Singleton().serverLoop();

    return 0;
}
