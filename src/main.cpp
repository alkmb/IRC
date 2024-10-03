#include "../includes/ircServer.hpp"
#include "../includes/Client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <poll.h>
#include <fcntl.h>

#define MAX_CLIENTS 100

int main() {
    Client client;
    ircServer server;

    server._serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    server.serverAddress.sin_family = AF_INET;
    server.serverAddress.sin_port = htons(server._port);
    server.serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(server._serverSocket, (struct sockaddr*)&server.serverAddress, sizeof(server.serverAddress)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    if (listen(server._serverSocket, 5) < 0) {  // Allow up to 5 pending connections
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;  // Number of active file descriptors

    fds[0].fd = server._serverSocket;
    fds[0].events = POLLIN;  // Check for incoming connections

    while (true) {
        int poll_count = poll(fds, nfds, -1);  // Wait indefinitely until an event occurs
        if (poll_count < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }
        if (fds[0].revents & POLLIN) {
            int new_socket = accept(server._serverSocket, NULL, NULL);
            if (new_socket < 0) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }
            std::cout << "New client connected, socket fd: " << new_socket << std::endl;
            if (nfds < MAX_CLIENTS) {
                fds[nfds].fd = new_socket;
                fds[nfds].events = POLLIN;  // Monitor for incoming data
                nfds++;
            } else
                close(new_socket);
        }
        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int bytesReceived = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytesReceived == 0) {
                    std::cout << "Client disconnected, socket fd: " << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = fds[nfds - 1].fd;
                    nfds--;
                } else {
                    buffer[bytesReceived] = '\0';
                    std::cout << "Received message: " << buffer << std::endl;
                    send(fds[i].fd, buffer, bytesReceived, 0);  // Echo the message back
                }
            }
        }
    }

    // Close the server socket (unlikely to reach here in practice)
    close(server._serverSocket);

    return 0;
}

