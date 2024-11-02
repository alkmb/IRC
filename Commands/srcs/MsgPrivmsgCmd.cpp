#include "../includes/MsgPrivmsgCmd.hpp"

MsgPrivmsgCmd::MsgPrivmsgCmd() {}

MsgPrivmsgCmd::~MsgPrivmsgCmd() {}

bool MsgPrivmsgCmd::validate(IRCMessage const &message)
{
    Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
    std::string chars = message.getTrailing();
    if (message.getParams().size() == 0)
    {
        Server::Singleton().sendMsg(client, "411 ERR_NORECIPIENT :No recipient given (PRIVMSG)\r\n");
        return false;
    }
    if (chars.empty())
    {
        Server::Singleton().sendMsg(client, "412 ERR_NOTEXTTOSEND :No text to send\r\n");
        return false;
    }
    execute(client, message);
    return true;
}
//enviar el mensaje a todos los del canal
void MsgPrivmsgCmd::execute(Client *client, IRCMessage const &message)
{
    std::string targetNick = message.getParams()[0];
    std::string msgContent = message.getTrailing();
    //std::cout << "nick es --> " << client->getNickName() << std::endl;
    Client *targetClient = Server::Singleton().getClientByNickName(targetNick);
    if (targetClient)
    {
        std::string fullMsg = ":" + client->getNickName() + " PRIVMSG " + targetNick + " :" + msgContent + "\r\n";
        Server::Singleton().sendMsg(targetClient, fullMsg);
    }
    else
    {
        Channel *chan = Server::Singleton().getChannelByName(targetNick);
        if (chan)
        {
            chan->sendMsgExcept(client, ":" + client->getNickName() + " PRIVMSG " + chan->getChannelName() + " :" + message.getTrailing() + "\r\n");
        }
        else
            Server::Singleton().sendMsg(client, "401 ERR_NOSUCHNICK :No such nick/channel\r\n");
    }
}