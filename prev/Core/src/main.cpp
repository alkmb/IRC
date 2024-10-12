#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

int main() {

    // Initialize and run the server
    Server& server = Server::Singleton();
    if (server.initialize("password", 8080) != 0) {
        std::cerr << "Server initialization failed" << std::endl;
        return -1;
    }
    server.serverLoop();
    return 0;
}