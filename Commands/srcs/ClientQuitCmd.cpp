#include "../includes/ClientQuitCmd.hpp"
#include "../../Core/includes/Server.hpp"

QuitCommand::QuitCommand()
{
}

QuitCommand::~QuitCommand()
{
}

void QuitCommand::execute(Client *client, IRCMessage const &message)
{
    message.print();
    client->getFd()->fd = -1;
}

bool QuitCommand::validate(IRCMessage const &message)
{
    struct pollfd *cliFd = Server::Singleton().getCurrentFd();
    Client *client = Server::Singleton().getClientByFd(cliFd);

    if (message.getCommand().empty())
    {
        Server::Singleton().sendMsg(client, " :Not Leaving\r\n");
        return false;
    }

    std::string msg = ":" + client->getNickName() + " QUIT :" + message.getTrailing() + "\r\n";
    execute(client, message);
    return true;
}
