#include <stdlib.h>
#include"CServer.h"
using std::string;

#define PORT 8080
#define UCMD_DEFAULT_DELIMETER " "

using namespace std;

CServer::CServer(void){
    
}

CServer::~CServer(void){

}

bool CServer::Creat_Socket(void)
{
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
	return true;
}
void CServer::Identify_socket()
{
 	address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
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
}

int CServer::ConnectServer(void)
{
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
	return 0;
}

void CServer::SendData(char* message)
{
	send(new_socket, message, strlen(message),0);
}

int CServer::CheckSocket(void)
{

}

void CServer::ReceiveData(char* msg, int msg_length)
{
    valread = read( new_socket , msg, msg_length);

}

void CServer::EndConnection(void)
{
    close(new_socket);
}


