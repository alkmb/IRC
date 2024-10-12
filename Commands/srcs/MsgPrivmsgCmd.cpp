#include "../includes/MsgPrivmsgCmd.hpp"
#include "../../Core/includes/Server.hpp"

MsgPrivmsgCmd::MsgPrivmsgCmd() {}

MsgPrivmsgCmd::~MsgPrivmsgCmd() {}

bool MsgPrivmsgCmd::validate(IRCMessage const &message)
{
    Client *client = Server::Singleton().getClientByNickName(message.getPrefix());

    if (message.getParams().empty())
    {
        Server::Singleton().sendMsg(client, "411 ERR_NORECIPIENT :No recipient given (PRIVMSG)\r\n");
        return false;
    }
    if (message.getParams().size() < 1)
    {
        Server::Singleton().sendMsg(client, "412 ERR_NOTEXTTOSEND :No text to send\r\n");
        return false;
    }

    execute(client, message);
    return true;
}

void MsgPrivmsgCmd::execute(Client *client, IRCMessage const &message)
{

    std::string targetNick = message.getParams()[0];
    std::string msgContent = message.getParams()[1];
        std::cout << "validating message" << std::endl;

    Client *targetClient = Server::Singleton().getClientByNickName(targetNick);
    if (targetClient)
    {
        std::string fullMsg = ":" + client->getNickName() + " PRIVMSG " + targetNick + " :" + msgContent + "\r\n";
        Server::Singleton().sendMsg(targetClient, fullMsg);
        Server::Singleton().sendMsgAll(fullMsg);
    }
    else
    {
        Server::Singleton().sendMsg(client, "401 ERR_NOSUCHNICK " + msgContent + " :No such nick/channel\r\n");
    }

}