#include "../includes/Client.hpp"

Client::Client()
{

}

Client::~Client()
{

}

std::string Client::getNickName()
{
    return this->_nick;
}

std::string Client::getRealName()
{
    return this->_real;
}

bool    Client::isAdmin()
{
    return this->_isAdmin;
}

struct pollfd   *Client::getFd()
{
    return this->_fd;
}

void    Client::setFd(struct pollfd *fd)
{
    this->_fd = fd;
}

void    Client::setNick(const std::string &name)
{
    this->_nick = name;
}

void    Client::setReal(const std::string &name)
{
    this->_real = name;
}

void    Client::setAdmin(bool admin)
{
    this->_isAdmin;
}