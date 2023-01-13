#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>       
#include <string.h>
#include <sys/time.h> // libraries to use linux timer signal 
#include <signal.h>
#include <fcntl.h>

using namespace std;
#define PORT 8080

int main()
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct timeval timeout;
    char buffer[30000] = {0};

    timeout.tv_sec = 1; // Time out after a minute
    timeout.tv_usec = 0;
    fd_set read_set;
    char *hello = "19 20 30 15";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    int flags = fcntl(server_fd, F_GETFL);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.65");
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
   
            
    while(1)
    {
        
        int new_socket = accept(server_fd, NULL, NULL);
        if (new_socket == -1) {
            if (errno == EWOULDBLOCK) {
                printf("No pending connections; sleeping for one second.\n");
                sleep(1);
            } else {
                perror("error when accepting connection");
                exit(1);
            }
        }
        /*if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }*/
        FD_ZERO(&read_set);
        FD_SET(new_socket, &read_set);

        int r=select(new_socket+1, &read_set, NULL, NULL, &timeout);

        if( r<0 ) {
            // Handle the error
            cout<<"Error!!"<<endl;
        }

        if( r==0 ) {
            // Timeout - handle that. You could try waiting again, close the socket...
            cout<<"timeout!!"<<endl;
        }

        if( r>0 ) {
            read( new_socket , buffer, 30000);
            printf("%s\n",buffer );
            if (!strcmp(buffer,"LGAna1234"))
                send(new_socket , "T" , 1,0);
        }
        
        close(new_socket);
        
    }
    return 0;
}