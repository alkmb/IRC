#include "../includes/ircServer.hpp"


ircServer::ircServer()
{
  this->_port = 8080;
  std::cout << "ircServer created with default constructor" << std::endl;
}


ircServer::ircServer(const ircServer &obj)
{
  std::cout << "ircServer created with copy constructor" << std::endl;
  *this = obj;
}

ircServer& ircServer::operator=(const ircServer &obj)
{
  std::cout << "ircServer created with copy assignment operator" << std::endl;
  if (this == &obj)
    this->_port = obj._port;
  return *this;
}

ircServer::~ircServer()
{
  std::cout << "ircServer has been destroyed" << std::endl;
}
