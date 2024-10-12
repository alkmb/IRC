#pragma once
#include "Channel.hpp"

class Server {
private:
    std::vector<struct pollfd> _fds;
    std::vector<Channel> _channels;
    std::vector<Client> _clients;
    struct pollfd _newFd;
    int _pollCount;
    int _serverSocket;
    int _newSocket;
    std::string _passwd;
    unsigned short _port;
    bool _endServer;
    sockaddr_in _serverAddress;

public:
    Server();
    ~Server();

    static Server& Singleton()
    {
        static Server instance;
        return instance;
    }

        Server& operator+=(std::string const& chan);
        int initialize(const std::string &psswd, const unsigned short &port);
        void serverLoop();
        Channel* getChannelByName(const std::string &name);
        struct pollfd *getClientFdByNickName(const std::string &name);
        struct pollfd *getClientFdByRealName(const std::string &name);
        Client* getClientByNickName(const std::string &name);
        Client* getClientByRealName(const std::string &name);
        int getServerSocket();
        int sendMsg(Client* client, const std::string &msg);
        int sendMsgAll(const std::string &msg);
        void createChannel(const std::string &name);
        void createClient(const std::string &nick, const std::string &real, struct pollfd *fd);
        int addClientToChannel(Client *client, Channel *channel);
        int moveClientFromToChannel(Client *client, Channel *from, Channel *to);
        Channel *getChannelByClient(Client *client);
        int removeClientFromChannel(Client *client, Channel *channel);
};