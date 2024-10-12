#pragma once

#include "ICommand.hpp"

class MessageCommand : public ICommand
{
protected:
    void sendToUser();
    void sendToChannel();

public:
    MessageCommand();
    virtual ~MessageCommand();

    virtual void execute(Client *client, IRCMessage const &message) = 0;
    virtual bool validate(IRCMessage const &message) = 0;
};
