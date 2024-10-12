#include "../includes/Server.hpp"

Server::Server() : _endServer(false) 
{

}

Server::~Server() 
{

}


int Server::initialize(const std::string &psswd, const unsigned short &port)
{
    this->_passwd = psswd;
    this->_port = port;
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocket == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->_serverAddress.sin_port = htons(this->_port);

    if (bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) < 0)
    {
        std::cerr << "Bind to Port Failed" << std::endl;
        return -1;
    }

    if (listen(this->_serverSocket, 3) < 0)
    {
        std::cerr << "Listen Error" << std::endl;
        return -1;
    }

    struct pollfd serverFd;
    serverFd.fd = this->_serverSocket;
    serverFd.events = POLLIN;
    this->_fds.push_back(serverFd);

    return 0;
}

void Server::serverLoop()
{
    createChannel("general");

    while (!_endServer)
    {
        _pollCount = poll(_fds.data(), _fds.size(), -1);
        if (_pollCount < 0)
        {
            std::cerr << "Poll error" << std::endl;
            break;
        }
        for (size_t i = 0; i < _fds.size(); ++i)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _serverSocket)
                {
                    // Handle new connection
                    _newSocket = accept(_serverSocket, nullptr, nullptr);
                    if (_newSocket < 0)
                    {
                        std::cerr << "Accept error" << std::endl;
                        continue;
                    }
                    _newFd.fd = _newSocket;
                    _newFd.events = POLLIN;
                    _fds.push_back(_newFd);
                    // createClient("user1", "User One", nullptr);
                }
                else
                {
                    char buffer[1024];
                    int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
                    if (bytesRead <= 0)
                    {
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        --i;
                    }
                    else
                    {
                        buffer[bytesRead] = '\0';
                        std::cout << "Received message: " << buffer << std::endl;
                    }
                }
            }
        }
    }
}

Server& Server::operator+=(std::string const& chanName)
{
    if (getChannelByName(chanName) == nullptr)
        createChannel(chanName);
    return *this;
}

Channel    *Server::getChannelByName(const std::string &name)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i].getChannelName() == name)
            return &this->_channels[i];
    }
    return (nullptr);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return this->_clients[i].getFd();
    }
    return (nullptr);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return this->_clients[i].getFd();
    }
    return (nullptr);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return &this->_clients[i];
    }
    return (nullptr);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return &this->_clients[i];
    }
    return (nullptr);
}

int    Server::getServerSocket()
{
    return this->_serverSocket;
}

int Server::sendMsg(Client* client, const std::string &msg)
{
    send(client->getFd()->fd, msg.c_str(), msg.length(), 0);
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
    newChannel.setName(name);
    this->_channels.push_back(newChannel);
}

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd *fd)
{
    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setFd(fd);
    newClient.setAdmin(false);
    this->_clients.push_back(newClient);
}

int Server::addClientToChannel(Client *client, Channel *channel)
{
    if (client != nullptr)
    {
        (*channel->getClientsFromChannel()).push_back(client);
        if (client->isAdmin())
        {
            (*channel->getAdmins()).push_back(client);
        }
        return 0;
    }
    return -1;
}

int Server::moveClientFromToChannel(Client *client, Channel *from, Channel *to)
{
    if (client != nullptr)
    {
        (*from->getClientsFromChannel()).erase(std::find(begin(*from->getClientsFromChannel()), end(*from->getClientsFromChannel()), client));
        (*to->getClientsFromChannel()).push_back(client);
        if (client->isAdmin())
        {
            (*from->getAdmins()).erase(std::find(begin(*from->getAdmins()), end(*from->getAdmins()), client));
            (*to->getAdmins()).push_back(client);
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
    return nullptr;
}

int Server::removeClientFromChannel(Client *client, Channel *channel)
{
    if (client != nullptr)
    {
        std::vector<Client*> *clients = channel->getClientsFromChannel();
        auto it = std::find(begin(*clients), end(*clients), client);
        for (int i = 0; i < (*clients).size(); i++)
        {
            if ((*clients)[i] == client)
            {
                (*clients)[i] = nullptr;
                break;
            }
        }
        if (it != (*clients).end())
            (*clients).erase(std::remove((*clients).begin(), (*clients).end(), nullptr), (*clients).end());
        if (client->isAdmin())
        {
            std::vector<Client*> *admins = channel->getAdmins();
            auto it = std::find(begin(*admins), end(*admins), client);
            for (int i = 0; i < (*admins).size(); i++)
            {
                if ((*admins)[i] == client)
                {
                    (*admins)[i] = nullptr;
                    break;
                }
            }
            if (it != (*admins).end())
                (*admins).erase(std::remove((*admins).begin(), (*admins).end(), nullptr), (*admins).end());
        }
        return 0;
    }
    return -1;
}
