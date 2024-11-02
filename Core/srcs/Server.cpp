#include "../includes/Server.hpp"
#include <signal.h>
#include <set>
#include <stdexcept>
#include <sys/stat.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/IRCMessage.hpp"
#include "../../Commands/includes/AuthNickCmd.hpp"
#include "../../Commands/includes/AuthUserCmd.hpp"
#include "../../Commands/includes/AuthPassCmd.hpp"
#include "../../Commands/includes/MsgPrivmsgCmd.hpp"
#include "../../Commands/includes/ChnlJoinCmd.hpp"
#include "../../Commands/includes/ChnlPartCmd.hpp"
#include "../../Commands/includes/ClientQuitCmd.hpp"
#include "../../Commands/includes/ChnlModeCmd.hpp"
#include "../../Commands/includes/ChnlKickCmd.hpp"
#include "../../Commands/includes/ChnlInviteCmd.hpp"
#include "../../Commands/includes/ChnlTopicCmd.hpp"

Server::Server()
{
    this->_delBuffer = "";
}

Server::~Server()
{
    delete this->_commands["NICK"];
    delete this->_commands["PASS"];
    delete this->_commands["PRIVMSG"];
    delete this->_commands["JOIN"];
    delete this->_commands["QUIT"];
    delete this->_commands["NOTICE"];
    delete this->_commands["MODE"];
    delete this->_commands["KICK"];
    delete this->_commands["INVITE"];
    delete this->_commands["TOPIC"];
    delete this->_commands["PART"];
    delete this->_commands["USER"];
}

int Server::initialize(const std::string &psswd, const unsigned int &port)
{
    try
    {
        this->_commands["NICK"] = new AuthNickCmd();
        this->_commands["USER"] = new AuthUserCmd();
        this->_commands["PASS"] = new AuthPassCmd();
        this->_commands["PRIVMSG"] = new MsgPrivmsgCmd();
        this->_commands["JOIN"] = new ChnlJoinCmd();
        this->_commands["QUIT"] = new QuitCommand();
        this->_commands["MODE"] = new ChnlModeCmd();
        this->_commands["KICK"] = new ChnlKickCmd();
        this->_commands["INVITE"] = new ChnlInviteCmd();
        this->_commands["TOPIC"] = new ChnlTopicCmd();
        this->_commands["PART"] = new ChnlPartCmd();

        std::cout << this->_fds.size();
        this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (this->_serverSocket < 0)
            throw std::runtime_error("Error: Not able to create the socket.");
        int a = 1;
        if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) == -1)
            throw std::runtime_error("Not able to set the socket correctly.");
        this->_serverAddress.sin_family = AF_INET;
        this->_serverAddress.sin_port = htons(port);
        this->_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        this->_passwd = psswd;
        if (bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1) 
            throw std::runtime_error("Error: Not accessible port.");
        if (listen(this->_serverSocket, 100) == -1)
            throw std::runtime_error("Error: Not listening to connections.");
        struct pollfd server;
        this->_fds.push_back(server);
        get(this->_fds, 0)->fd = this->_serverSocket;
        get(this->_fds, 0)->events = POLLIN;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}

void closeall(int signum)
{
    // Handle signal to close all connections
    std::cout << "Closing all connections..." << std::endl;
    close(Server::Singleton().getServerSocket());
    exit(signum);
}

void Server::serverLoop()
{
    signal(SIGINT, closeall);
    int polVal, tmp_fd;
    while (1)
    {
        std::vector<struct pollfd> list = createVectorFromList(this->_fds);
        polVal = poll(list.data(), list.size(), -1);

        if (polVal == -1)
        {
            std::cout << "Error reading poll." << std::endl;
            break;
        }
        Server::Singleton()[0]->events = POLLIN;
        // counter = Server::Singleton().getFdSize();
        for (int i = 0; i < Server::Singleton().getFdSize(); i++)
        {
            Server::Singleton().setCurrentFd(Server::Singleton()[i]);
            if (list[i].revents == 0)
            {
                std::cout << "bucle\n";
                continue;
            }
            if (i != 0)
            {
                Server::Singleton()[i]->revents = 1;
                char buf[1024];
                int result = recv(Server::Singleton()[i]->fd, &buf, 1, MSG_PEEK);
                if (result == 0)
                {
                    std::ostringstream index;
                    index << i;
                    this->_delBuffer += index.str() + " ";
                    //i--;
                    //counter--;
                    continue;
                }
            }
            if (Server::Singleton()[i]->fd == Server::Singleton().getServerSocket())
            {
                // Function to accept connection and create client
                tmp_fd = accept(Server::Singleton().getServerSocket(), 0, 0);
                if (tmp_fd < 0)
                    continue;
                struct pollfd clientfd;
                clientfd.fd = tmp_fd;
                clientfd.events = POLLIN;
                Server::Singleton().createClient("", "", clientfd);
                Server::Singleton()[0]->events = POLLOUT;
            }
            else
            {
                char buffer[1024] = {0};
                int recVal = 0;
                recVal = recv(Server::Singleton()[i]->fd, buffer, sizeof(buffer), 0);
                if (!recVal)
                // TODO eliminal fd si recVal fall.
                {
                    break;
                }
                /*if (recVal < 0)
                {
                    //Server::Singleton()[i]->fd = -1;
                    Server::Singleton() -= Server::Singleton().getClientByFd(Server::Singleton()[i]);
                    Server::Singleton() -= Server::Singleton()[i];
                    //i--;
                    counter--;
                    continue;
                }*/
                //std::cout << buffer << std::endl;
                std::string str = buffer;
                Client *client = getClientByFd(Server::Singleton()[i]);
                client->addBuffer(str);
                std::string buf = client->getBuffer();
                std::cout << buf << "\n";
                if (buf.find('\r') != std::string::npos)
                {
                    client->clearBuffer();
                    std::stringstream ss(buf);
                    std::string line;
                    while (std::getline(ss, line, '\n'))
                    {
                        IRCMessage message(line);
                        Server::Singleton() *= message;
                        if (Server::Singleton()[i]->fd == -1)
                        {
                            std::ostringstream index;
                            index << i;
                            this->_delBuffer += index.str() + " ";
                            //Server::Singleton() -= Server::Singleton()[i];
                            //counter--;
                            break;
                        }
                    }
                }
                memset(buffer, 0, 1024);
            }
        }
        //borrado del bufer
        std::stringstream ss;
        ss.str (this->_delBuffer);
        std::string t;
        while (ss >> t) 
        {
            int iter = atoi(t.c_str());
            std::cout << "fd deleted index is --> " << iter << std::endl;
            if (Server::Singleton().getClientByFd(Server::Singleton()[iter]))
            {
                std::cout << "entra aqui\n";
                std::cout << "cliente borrado tenia nickname --> " << Server::Singleton().getClientByFd(Server::Singleton()[iter])->getNickName();
                Server::Singleton() -= Server::Singleton().getClientByFd(Server::Singleton()[iter]);
            }
            Server::Singleton()[iter]->fd = -1;
            Server::Singleton() -= Server::Singleton()[iter];
        }
        this->_delBuffer = "";
        //
    }
    // closing the socket.
    close(Server::Singleton().getServerSocket());
}

Server& Server::operator+=(std::string const& chanName)
{
    if (getChannelByName(chanName) == 0)
        createChannel(chanName);
    return *this;
}

Server& Server::operator-=(Client *client)
{
    //delete client
    close(client->getFd()->fd);
    client->getFd()->fd = -1;
    for (unsigned long i = 0; i < this->_channels.size(); i++)
        *get(this->_channels, i) -= client;
    std::list<Client>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), *client);
    if (it != this->_clients.end())
    {
        std::cout << "el viejo size de clients es " << this->_clients.size() << std::endl;
        std::cout << "cliente encontrado y borrado" << std::endl;
        this->_clients.erase(std::remove(this->_clients.begin(), this->_clients.end(), *client), this->_clients.end());
        std::cout << "el nuevo size de clients es " << this->_clients.size() << std::endl;
    }
    return *this;
}

Server& Server::operator-=(struct pollfd *fd)
{
    fd->fd = -1;
    std::list<struct pollfd>::iterator it;
    for (it = this->_fds.begin(); it != this->_fds.end(); it++)
    {
        if ((*it).fd == -1)
        {
            std::cout << "el viejo size de fds es " << this->_fds.size() << std::endl;
            this->_fds.erase(it);
            std::cout << "el nuevo size de fds es " << this->_fds.size() << std::endl;
            break;
        }
    }
    return *this;
}

Server&			Server::operator*=(IRCMessage const& msg)
{
    if (this->_commands.find(msg.getCommand()) != this->_commands.end())
        this->_commands[msg.getCommand()]->validate(msg);
    return *this;
}

struct pollfd *Server::operator[](unsigned long idx)
{
    return get(this->_fds, idx);
}

std::string Server::getPasswd()
{
    return this->_passwd;
}

struct pollfd   *Server::getCurrentFd()
{
    return this->_currentFd;
}

void			Server::setCurrentFd(struct pollfd *current)
{
    this->_currentFd = current;
}

Channel    *Server::getChannelByName(const std::string &name)
{
    for (unsigned long i = 0; i < this->_channels.size(); i++)
    {
        if (get(this->_channels, i)->getChannelName() == name)
            return get(this->_channels, i);
    }
    return (0);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if (get(this->_clients, i)->getNickName() == name)
            return get(this->_clients, i)->getFd();
    }
    return (0);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if (get(this->_clients, i)->getRealName() == name)
            return get(this->_clients, i)->getFd();
    }
    return (0);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if (get(this->_clients, i)->getNickName() == name)
            return get(this->_clients, i);
    }
    return (0);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if (get(this->_clients, i)->getRealName() == name)
            return get(this->_clients, i);
    }
    return (0);
}

Client			*Server::getClientByFd(struct pollfd *fd)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        if (get(this->_clients, i)->getFd() == fd)
            return get(this->_clients, i);
    }
    return (0);
}

int    Server::getServerSocket()
{
    return this->_serverSocket;
}

int				Server::getFdSize()
{
    return this->_fds.size();
}

int Server::sendMsg(Client* client, const std::string &msg)
{
    int fd = client->getFd()->fd;
    send(fd, msg.c_str(), msg.length(), 0);
    return 0;
}

int Server::sendMsgAll(const std::string &msg)
{
    for (unsigned long i = 0; i < this->_clients.size(); i++)
    {
        send(get(this->_clients, i)->getFd()->fd, msg.c_str(), msg.length(), 0);
    }
    return 0;
}

void Server::createChannel(const std::string &name)
{
	Channel newChannel;
	if (newChannel.setName(name))
	{
		std::cout << "paso pro aqui" << std::endl;
		struct pollfd *fd = Server::Singleton().getCurrentFd();
		Client *client = Server::Singleton().getClientByFd(fd);
		//newChannel.setChannelModes(client);
		this->_channels.push_back(newChannel);
		std::cout << "DEBUG 1" << std::endl;
		*get(this->_channels, this->_channels.size() - 1) += client;
		std::cout << "DEBUG 2" << std::endl;
		get(this->_channels, this->_channels.size() - 1)->getModes()->chanCreator = client->getNickName();
		std::cout << "DEBUG 3" << std::endl;
		get(this->_channels, this->_channels.size() - 1)->getModes()->topicLock = false;
		std::cout << "DEBUG 4" << std::endl;
		get(this->_channels, this->_channels.size() - 1)->getOperators()->push_back(client);
		std::cout << "DEBUG 5" << std::endl;
		std::cout << get(this->_channels, this->_channels.size() - 1)->getChannelName() << " created" << std::endl;
	}
}

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd fd)
{
    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setAdmin(false);
    this->_fds.push_back(fd);
    newClient.setFd(get(this->_fds, this->_fds.size() - 1));
    this->_clients.push_back(newClient);
}

int Server::addClientToChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        (*channel->getClientsFromChannel()).push_back(client);
        if (client->isOperator())
        {
            (*channel->getOperators()).push_back(client);
        }
        return 0;
    }
    return -1;
}

int Server::removeClientFromChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        std::list<Client *> *listChan = channel->getClientsFromChannel();
        std::list<Client *> *listOper = channel->getOperators();
        for (unsigned long i = 0; i < listChan->size(); i++)
        {
            if (*get(*listChan, i) == client)
            {
                deleteElementAtIndex(*listChan, i);
                std::cout << "Borrado el cliente de lista del canal\n";
                break;
            }
        }
        for (unsigned long i = 0; i < listOper->size(); i++)
        {
            if (*get(*listOper, i) == client)
            {
                deleteElementAtIndex(*listOper, i);
                std::cout << "Borrado el cliente de lista de operators\n";
                break;
            }
        }
        return 0;
    }
    return -1;
}

