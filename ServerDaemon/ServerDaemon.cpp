#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
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
#include <time.h>

#include "ServerMSGQ.h"

#define BUFFER_SIZE 256
#define PORT 8080

int main()
{
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];
    std::vector<int> client_sockets;
    int bytes_sent, total_bytes_sent = 0;
    pid_t pid, sid;
    int len, fd;

    //Message Queues
    char send_msg[MAX_MSG_LEN] = {0};
    char receive_msg[MAX_MSG_LEN] = {0};

    openlog("SPOT Daemon Test", LOG_PID, LOG_DAEMON);
    pid = fork(); // create a new process

    if (pid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        printf("Server Daemon PID: %d\n", pid);
        exit(EXIT_SUCCESS); // parent process (exit)
    }
    sid = setsid(); // create a new session

    if (sid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "setsid");
        exit(EXIT_FAILURE);
    }

    // make '/' the root directory
    if (chdir("/") < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "chdir");
        exit(EXIT_FAILURE);
    }

    create_message_queue();

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
            std::cout << "Num of clients: " << client_sockets.size() << std::endl;
        }

        // Check all client sockets for incoming data
        for (int i = 0; i < client_sockets.size(); i++) {
            int bytes_received = recv(client_sockets[i], buffer, BUFFER_SIZE, MSG_NOSIGNAL);
            
            // If there is information received send through messsage queue to main process
            if(bytes_received)
            {
                //std::cout << "Message Received " << client_sockets[i] << std::endl;
                //std::cout << bytes_received << buffer << std::endl;
                send_messagequeue(buffer);
                memset(buffer,0,BUFFER_SIZE);
                //std::cout << "On receive: " << CheckNumMsg2() << std::endl;
            }
        }

        memset(buffer,0,BUFFER_SIZE);
        for (int i = 0; i < client_sockets.size(); i++) {
            //sprintf(buffer,"Buffer: %i\n", j);
            bytes_sent = send(client_sockets[i], buffer, BUFFER_SIZE, MSG_NOSIGNAL);     
            if(bytes_sent == -1)
            {
                close(client_sockets[i]);
                client_sockets.erase(client_sockets.begin() + i);
                std::cout << "Client disconnected" << std::endl;
                //std::cout << "On send " << CheckNumMsg() << std::endl;
            }
            total_bytes_sent =+ bytes_sent;
        }

        // If there is content in msg queue to send
        if(CheckNumMsg())
        {
            // Put msg queue content in buffer
            receive_messagequeue(buffer);

            // Check all client sockets and send data
            for (int i = 0; i < client_sockets.size(); i++) {
                //sprintf(buffer,"Buffer: %i\n", j);
                bytes_sent = send(client_sockets[i], buffer, BUFFER_SIZE, MSG_NOSIGNAL);     
                if(bytes_sent == -1)
                {
                    close(client_sockets[i]);
                    client_sockets.erase(client_sockets.begin() + i);
                    std::cout << "Client disconnected" << std::endl;
                    //std::cout << "On send " << CheckNumMsg() << std::endl;
                }
                total_bytes_sent =+ bytes_sent;
            }
            if(total_bytes_sent)
            {
                total_bytes_sent = 0;
                memset(buffer,0,BUFFER_SIZE);
            }
        }
        //std::cout << "On Final " << CheckNumMsg() << std::endl;
        usleep(50000);

    }
    std::cout << "Server is down!" << std::endl;
    return 0;
}