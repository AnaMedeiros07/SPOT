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

    void Indentify_socket(char*);

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
      int insertDataUserID(const char* ,string ,string ,string );
      int insertDataUserEntryID(const char* ,string ,string );
    //___________________________________________________
    //______________Update Values in Database___________________
      int updateUserNumber(const char* ,string , string );
      int updateSensorValue(const char* ,string ,string );
      int updateSensorUpperLimit(const char* s,string ,string );
      int updateSensorLowerLimit(const char* ,string ,string );
      string GetNumber(const char* s,string name);
    //______________________Auxiliar Function_________________________________
    string quotesql(const string& );
    
    bool checklogin(const char* s,string name,string password);

    char* ReceiveData(void);

    void acquiredata(void);

    bool Updatelocalhouse(double, double, double, double);

    bool ProcessRequest(string);

    bool SendValuesOutOfRange(void); //definir o tipo de dados limits e trocar void

    bool SendImage(void); //trocar void pelo tipo da imagem

};

#endif