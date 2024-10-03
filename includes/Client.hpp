#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
  public:
    Client();
    Client(const Client &obj);
    Client& operator=(const Client &obj);
    ~Client();
    void setName(std::string name);
    void setUsername(std::string username);
    void setPassword(std::string password);
    std::string getName();
    std::string getUsername();
    std::string getPassword();

  private:
    std::string name;
    std::string username;
    std::string password;
};
#endif 
