#include "../includes/Server.hpp"
#include <signal.h>


#include "../includes/IRCMessage.hpp"
#include "../../Commands/includes/AuthNickCmd.hpp"
#include "../../Commands/includes/ClientQuitCmd.hpp"
#include "../../Commands/includes/AuthPassCmd.hpp"
#include "../../Commands/includes/MsgNoticeCmd.hpp"
#include "../../Commands/includes/MsgPrivmsgCmd.hpp"
#include "../../Commands/includes/ChnlJoinCmd.hpp"
#include "../../Commands/includes/ChnlWhoCmd.hpp"

Server::Server()
{

}

Server::~Server()
{
    delete this->_commands["NICK"];
    delete this->_commands["PASS"];
    delete this->_commands["PRIVMSG"];
    delete this->_commands["JOIN"];
    delete this->_commands["WHO"];
    delete this->_commands["QUIT"];
    delete this->_commands["NOTICE"];
}

#include <set>
#include <stdexcept>

int Server::initialize(const std::string &psswd, const unsigned short &port)
{
    // con el set no se duplican parametros Y se ordenan
    std::set<unsigned short> occupiedPorts = {80, 443, 21, 22, 25, 110, 143, 993, 995};
    if (occupiedPorts.find(port) != occupiedPorts.end()) {
        throw std::runtime_error("Port " + std::to_string(port) + " is commonly occupied. Please choose a different port.");
    }
    this->_commands["NICK"] = new AuthNickCmd();
    this->_commands["PASS"] = new AuthPassCmd();
    this->_commands["PRIVMSG"] = new MsgPrivmsgCmd();
    this->_commands["NOTICE"] = new MsgNoticeCmd();
    this->_commands["JOIN"] = new ChnlJoinCmd();
    this->_commands["WHO"] = new ChnlWhoCmd();
    this->_commands["QUIT"] = new QuitCommand();
    
    std::cout << this->_fds.size();
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int a;
    setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(port);
    this->_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_passwd = psswd;
    bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress));
    listen(this->_serverSocket, 5);
    struct pollfd server;
    this->_fds.push_back(server);
    this->_fds[0].fd = this->_serverSocket;
    this->_fds[0].events = POLLIN;
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
    int polVal, tmp_fd, counter;
    while (1)
    {
        polVal = poll(Server::Singleton()[0], Server::Singleton().getFdSize(), -1);
        std::cout << Server::Singleton().getFdSize() << std::endl;
        Server::Singleton()[0]->events = POLLIN;
        counter = Server::Singleton().getFdSize();
        for (int i = 0; i < counter; i++)
        {
            Server::Singleton().setCurrentFd(Server::Singleton()[i]);
            if (Server::Singleton()[i]->revents == 0)
                continue;
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
                std::cout << buffer << std::endl;
                std::string str = buffer;
                std::stringstream ss(str);
                std::string line;
                while (std::getline(ss, line, '\n'))
                {
                    IRCMessage message(line);
                    message.print();
                    // if (message.getIsValid())
                    Server::Singleton() *= message;
                    if (Server::Singleton()[i]->fd == -1)
                    {
                        Server::Singleton() -= Server::Singleton()[i];
                        break;
                    }
                }
                memset(buffer, 0, 1024);
            }
        }
    }
    // closing the socket.
    close(Server::Singleton().getServerSocket());
    for (int i = 0; i < Server::Singleton().getFdSize(); i++)
    {
        close(Server::Singleton()[i]->fd);
    }
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
    client->getFd()->fd = -1;
    for (int i = 0; i < this->_channels.size(); i++)
        this->_channels[i] -= client;
    std::deque<Client>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), *client);
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
    std::deque<struct pollfd>::iterator it;
    for (it = this->_fds.begin(); it < this->_fds.end(); it++)
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

struct pollfd *Server::operator[](int idx)
{
    return &this->_fds[idx];
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
    for (int i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i].getChannelName() == name)
            return &this->_channels[i];
    }
    return (0);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return this->_clients[i].getFd();
    }
    return (0);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return this->_clients[i].getFd();
    }
    return (0);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        std::cout << "comparando " << this->_clients[i].getNickName() << " con " << name << std::endl;
        if (this->_clients[i].getNickName() == name)
            return &this->_clients[i];
    }
    return (0);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return &this->_clients[i];
    }
    return (0);
}

Client			*Server::getClientByFd(struct pollfd *fd)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getFd() == fd)
            return &this->_clients[i];
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
    for (int i = 0; i < this->_clients.size(); i++)
    {
        send(this->_clients[i].getFd()->fd, msg.c_str(), msg.length(), 0);
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
		this->_channels[this->_channels.size() - 1] += client;
		std::cout << "DEBUG 2" << std::endl;
		this->_channels[this->_channels.size() - 1].getModes()->chanCreator = client->getNickName();
		std::cout << "DEBUG 3" << std::endl;
		this->_channels[this->_channels.size() - 1].getModes()->topicChannel = true;
		std::cout << "DEBUG 4" << std::endl;
		this->_channels[this->_channels.size() - 1].getOperators()->push_back(client);
		std::cout << "DEBUG 5" << std::endl;
		std::cout << this->_channels[this->_channels.size() - 1].getChannelName() << " created" << std::endl;
	}
}

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd fd)
{
    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setAdmin(false);
    this->_fds.push_back(fd);
    newClient.setFd(&this->_fds[this->_fds.size() - 1]);
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

Channel *Server::getChannelByClient(Client *client)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        for (int j = 0; j < (*this->_channels[i].getClientsFromChannel()).size(); j++)
        {
            if ((*this->_channels[i].getClientsFromChannel())[j] == client)
                return &this->_channels[i];
        }
    }
    return 0;
}

int Server::removeClientFromChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        std::deque<Client*> *clients = channel->getClientsFromChannel();
        std::deque<Client*>::iterator it = std::find((*clients).begin(), (*clients).end(), client);
        for (int i = 0; i < (*clients).size(); i++)
        {
            if ((*clients)[i] == client)
            {
                (*clients)[i] = 0;
                break;
            }
        }
        if (it != (*clients).end())
            (*clients).erase(std::remove((*clients).begin(), (*clients).end(), (Client*)0), (*clients).end());
        if (client->isOperator())
        {
            std::deque<Client*> *admins = channel->getOperators();
            std::deque<Client*>::iterator it2 = std::find((*admins).begin(), (*admins).end(), client);
            for (int i = 0; i < (*admins).size(); i++)
            {
                if ((*admins)[i] == client)
                {
                    (*admins)[i] = 0;
                    break;
                }
            }
            if (it2 != (*admins).end())
                (*admins).erase(std::remove((*admins).begin(), (*admins).end(), (Client*)0), (*admins).end());
        }
        return 0;
    }
    return -1;
}
