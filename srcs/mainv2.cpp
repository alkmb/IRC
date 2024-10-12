// C++ program to show the example of server application in
// socket programming
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>

#include "../Core/includes/IRCMessage.hpp"
#include "../Core/includes/Server.hpp"

int main(int argc, char *argv[])
{
	int polVal, tmp_fd, counter;
	Server::Singleton().initialize(argv[2], (unsigned short)atoi(argv[1]));
	while (1){
		polVal = poll(Server::Singleton()[0], Server::Singleton().getFdSize(), -1);
		std::cout << Server::Singleton().getFdSize() << std::endl;
		Server::Singleton()[0]->events = POLLIN;
		counter = Server::Singleton().getFdSize();
		for (int i = 0; i < counter; i++)
		{
			Server::Singleton().setCurrentFd(Server::Singleton()[i]);
			if(Server::Singleton()[i]->revents == 0)
			    continue;
			if (Server::Singleton()[i]->fd == Server::Singleton().getServerSocket())
			{
				tmp_fd = accept(Server::Singleton().getServerSocket(), 0, 0);
				if (tmp_fd < 0)
					break;
				struct pollfd clientfd;
				clientfd.fd = tmp_fd;
				clientfd.events = POLLIN;
				Server::Singleton().createClient("", "", clientfd);
				Server::Singleton()[0]->events = POLLOUT;
			}
			else
			{
				char buffer[1024] = { 0 };
				int recVal = 0;
				recVal = recv(Server::Singleton()[i]->fd, buffer, sizeof(buffer), 0);
				std::cout << buffer << std::endl;
				std::string str = buffer;
				std::stringstream ss(str);
    			std::string line;
    			while(std::getline(ss,line,'\n'))
				{
					IRCMessage message(line);
					message.print();
					//if (message.getIsValid())
						//Server::Singleton() *= message;
    			}
				memset(buffer,0,1024);
			}
		}
	}
	// closing the socket.
	close(Server::Singleton().getServerSocket());

	return 0;
}
