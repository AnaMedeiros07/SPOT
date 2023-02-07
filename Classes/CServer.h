#ifndef __CSERVER_H_
#define __CSERVER_H_

#include <string.h>
#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>       
#include <fcntl.h>
#include <algorithm>
#include <bits/stdc++.h>

#include "sqlite3.h"

using std::string;

//extern Command my_cmd_list;

class CServer
{
    private:

    int server_fd, new_socket; 
    long valread;
    struct timeval timeout;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    public:

    CServer();

    ~CServer(void);
    //---------------For http connection----------------
    bool Create_Socket(void);
    void Identify_socket();
    int ConnectServer(void);
    void EndConnection(void);
    void SendData(char*);
    int CheckSocket(void);
    void ReceiveData(char*, int);
    
};

#endif