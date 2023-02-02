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
#include <stdlib.h>
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
    bool Creat_Socket(void);
    void Identify_socket();
    void ConnectServer(void);
    void EndConnection(void);
    void SendData(char*);
    //-----------------For Database ----------------------------------
    //____________create tables_________________________
      int createDB(const char* );
      int createTable(const char* );
    //______________________________________________
    //_______________Insert in the database__________________
      int insertDataEventID(const char*,string ,string,string, string,string);
      int insertDataSensorID(const char* ,string ,string ,string ,string ,string );
      int insertDataHouseID(const char* ,string );
      int insertDataUserID(string ,string ,string );
      int insertDataUserEntryID(const char* ,string ,string );
    //___________________________________________________
    //______________Update Values in Database___________________
      int updateUserNumber(string , string );
      int updateUserSensor(string value,string type,string user );
      int updateUserLimits(string lower_limit,string upper_limit,string type,string user );
      char* GetNumber(string name);
      char* GetUserID(string name );
    //______________________Auxiliar Function_________________________________
    string quotesql(const string& );
    int checklogin(string name,string password);
    char* ReceiveData(void);
    char* ProcessRequest(string);
    

};

#endif