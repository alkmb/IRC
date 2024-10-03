#include "../includes/Client.hpp"

Client::Client() {
  std::cout << "Client created with default constructor" << std::endl;
}

Client::Client(const Client &obj)
{
  std::cout << "Client created with copy constructor" << std::endl;
  *this = obj;
}

Client& Client::operator=(const Client &obj)
{
  std::cout << "Client created with copy assignment operator" << std::endl;
  if (this == &obj)
  {
    this->name = obj.name;
    this->username = obj.username;
    this->password = obj.password;
  } 
  return *this;
}

Client::~Client()
{
  std::cout << "Client has been destroyed" << std::endl;
}

void Client::setName(std::string name)
{
  this->name = name;
  std::cout << "Name has been set to " << name << std::endl;
}

void Client::setUsername(std::string username)
{
  this->username = username;
  std::cout << "Username has been set to " << username << std::endl;
}

void Client::setPassword(std::string password)
{
  this->password = password;
  std::cout << "Password has been set " << std::endl;
}

std::string Client::getName()
{
  return this->name;
}

std::string Client::getUsername()
{
  return this->username;
}

std::string Client::getPassword()
{
  return this->password;
}
