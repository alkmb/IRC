#include "../includes/Client.hpp"

Client::Client()
{
    this->_isVerified = false;
    this->_correctPwd = false;
}

Client::~Client()
{

}

bool			Client::operator==(const Client &n2)
{
    return (this == &n2);
}

void			Client::setPwd()
{
    this->_correctPwd = true;
}

bool			Client::correctPwd()
{
    return this->_correctPwd;
}

std::string Client::getNickName()
{
    return this->_nick;
}

std::string Client::getRealName()
{
    return this->_real;
}

bool    Client::isVerified()
{
    return this->_isVerified;
}

void    Client::setVerified()
{
    this->_isVerified = true;
}

bool    Client::isOperator()
{
    return this->_isOperator;
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

void    Client::setAdmin(bool op)
{
    this->_isOperator = op;
}

void	Client::addBuffer(const std::string &append)
{
	this->_buffer += append;
}

void	Client::clearBuffer()
{
	this->_buffer = "";
}

std::string		Client::getBuffer()
{
	return this->_buffer;
}