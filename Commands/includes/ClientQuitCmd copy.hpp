#pragma once

#include "ICommand.hpp"

class QuitCommand : public ICommand
{
public:
    QuitCommand();
    ~QuitCommand();

    void execute(Client *client, IRCMessage const &message);
    bool validate(IRCMessage const &message);
};
