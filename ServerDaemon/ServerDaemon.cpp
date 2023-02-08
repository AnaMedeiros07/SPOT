#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ServerMSGQ.h"

#define BUFFER_SIZE 1024
#define PORT 8080

int main()
{
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];
    std::vector<int> client_sockets;
    int j = 10;
    int bytes_sent, total_bytes_sent = 0;

    // Create a server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error: could not create socket" << std::endl;
        return 1;
    }

    // Make the server socket non-blocking
    if (fcntl(server_socket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Error: could not set server socket as non-blocking" << std::endl;
        return 1;
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    // Bind the server socket to the address
    if (bind(server_socket, (sockaddr*) &server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: could not bind socket" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error: could not listen on socket" << std::endl;
        return 1;
    }

    std::cout << "Listening for incoming connections on port 8080..." << std::endl;

    memset(buffer,0,BUFFER_SIZE);

    // Keep the server running and keep accepting incoming connections
    while (true) {
        client_socket = accept(server_socket, (sockaddr*) &client_address, &client_len);

        // If a new connection is available, accept it
        if (client_socket >= 0) {
            // Make the client socket non-blocking
            if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0) {
                std::cerr << "Error: could not set client socket as non-blocking" << std::endl;
                return 1;
            }

            // Add the client socket to the list of client sockets
            client_sockets.push_back(client_socket);
            std::cout << "Accepted incoming connection from " << inet_ntoa(client_address.sin_addr) << std::endl;
        }

        // Check all client sockets for incoming data
        for (int i = 0; i < client_sockets.size(); i++) {
            int bytes_received = recv(client_sockets[i], buffer, BUFFER_SIZE, 0);
        }

        // If there is content to send
        if(j)
        {
            // Check all client sockets and send data
            for (int i = 0; i < client_sockets.size(); i++) {
                sprintf(buffer,"Buffer: %i\n", j);
                bytes_sent = send(client_sockets[i], buffer, BUFFER_SIZE, MSG_NOSIGNAL);     
                if(bytes_sent == -1)
                {
                    close(client_sockets[i]);
                    client_sockets.erase(client_sockets.begin() + i);
                    std::cout << "Client disconnected" << std::endl;
                }
                total_bytes_sent =+ bytes_sent;
            }
            if(total_bytes_sent)
            {
                j--;
                total_bytes_sent = 0;
                sleep(1);
            }
        }
        std::cout << client_sockets.size() << std::endl;
        sleep(1);
        //reload send buffer
        if(!j)
            j = 10;
    }

    return 0;
}