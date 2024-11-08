#include "../includes/MsgNoticeCmd.hpp"

MsgNoticeCmd::MsgNoticeCmd() 
{

}

MsgNoticeCmd::~MsgNoticeCmd() 
{

}

bool MsgNoticeCmd::validate(IRCMessage const &message)
{
    Client *client = Server::Singleton().getClientByNickName(message.getPrefix());

    if (message.getParams().empty())
    {
        Server::Singleton().sendMsg(client, "411 ERR_NORECIPIENT :No recipient given (PRIVMSG)\r\n");
        return false;
    }
    if (message.getTrailing().empty())
    {
        Server::Singleton().sendMsg(client, "412 ERR_NOTEXTTOSEND :No text to send\r\n");
        return false;
    }
    execute(client, message);
    return true;
}


void MsgNoticeCmd::execute(Client *client, IRCMessage const &message)
{
    std::string targetNick = message.getParams()[0];
    std::string msgContent = message.getTrailing();
    //std::cout << "Mensaje privado de " << client->getNickName() << " a " << targetNick << ": " << msgContent << std::endl;

    Client *targetClient = Server::Singleton().getClientByNickName(targetNick);
    if (targetClient)
    {
        std::string fullMsg = ":" + client->getNickName() + " NOTICE " + targetNick + " :" + msgContent + "\r\n";
        Server::Singleton().sendMsg(client, fullMsg);
    }
    else
    {
        Channel *chan = Server::Singleton().getChannelByName(targetNick);
        if (chan)
        {
            chan->sendMsgExcept(client, ":" + client->getNickName() + " NOTICE " + chan->getChannelName() + " :" + message.getTrailing() + "\r\n");
        }
        else
            Server::Singleton().sendMsg(client, "401 ERR_NOSUCHNICK :No such nick/channel\r\n");
    }
}