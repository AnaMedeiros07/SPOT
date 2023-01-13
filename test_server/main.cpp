#include"CServer.h"
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include <sys/time.h> // libraries to use linux timer signal 
#include <unistd.h>
#include <signal.h>
using namespace std;

#define TIME_USEC 4*10000
#define TIME_SEC 0

static volatile sig_atomic_t gotSigio = 0;




int main()
{ 
    CServer server;

    struct itimerval itv;
    const char* DATABASE = "DATABASE.db";
    char* receive;
    const char * word [15]={0};
    int i=0;
    server.createDB(DATABASE);
    server.createTable(DATABASE);

    server.Creat_Socket();

    server.Indentify_socket("192.168.1.71");

    while(1)
    {
        server.ConnectServer();
        //server.SendData("14 13 2 5");
        
        //usleep(5000000);

        //server.SendData("hello");

        //usleep(5000000);
        
        //server.SendData("teste");
       server.ReceiveData();
    
       
        server.EndConnection();
        
    }


    return 0;
}