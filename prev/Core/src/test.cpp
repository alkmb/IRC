#include "../includes/Server.hpp"
int main()
{
    Server::Singleton().createChannel("test");
    Server::Singleton().createClient("PEPE", "PEPE", nullptr);
    std::cout << "Channel name is " << Server::Singleton().getChannelByName("test")->getChannelName() << std::endl;
    Channel *chan = Server::Singleton().getChannelByName("test");
    Client *client= Server::Singleton().getClientByRealName("PEPE");
    *chan += *client;
    std::cout << "The first client of " << Server::Singleton().getChannelByName("test")->getChannelName()
    << " is " << (*chan->getClientsFromChannel())[0]->getNickName() << std::endl;
    *chan -= *client;
    std::cout << "client deleted from channel, size is now " << (*chan->getClientsFromChannel()).size() << std::endl;
    std::cout << "client was deleted from channel but is still present on the server, his name is "
    << client->getNickName() << std::endl;
    Server::Singleton() += "nuevoCanal";
    if (Server::Singleton().getChannelByName("nuevoCanal") != nullptr)
    {
        std::cout << "el nuevo canal es " << Server::Singleton().getChannelByName("nuevoCanal")->getChannelName() << std::endl;
    }
}