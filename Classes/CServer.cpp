#include"CServer.h"
using std::string;

#define PORT 8080
#define UCMD_DEFAULT_DELIMETER " "

using namespace std;

CServer::CServer(void){
    
}

CServer::~CServer(void){

}

bool CServer::Create_Socket(void)
{
    const int enable = 1;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            perror("setsockopt(SO_REUSEADDR) failed");
	
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
    //write(new_socket, message, strlen(message));
}

int CServer::CheckSocket(void)
{
    return 0;
}

void CServer::ReceiveData(char* msg, int msg_length)
{
    //valread = read( new_socket , msg, msg_length);
    int flags = fcntl(server_fd, F_GETFL);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
    fd_set read_set;

    struct timeval timeout;

    timeout.tv_sec = 1; // Time out after a minute
    timeout.tv_usec = 0;
    FD_ZERO(&read_set);
    FD_SET(new_socket, &read_set);

    int r=select(new_socket+1, &read_set, NULL, NULL, &timeout);
    //cout<<"wait to read"<<endl;
    if( r<0 ) {
        // Handle the error
        cout<<"Error!!"<<endl;
    }

    if( r==0 ) {
        // Timeout - handle that. You could try waiting again, close the socket...
        //cout<<"timeout!!"<<endl;
    }

    if( r>0 ) {
        read( new_socket , msg, msg_length);
        printf("%s\n",msg );
    }
    flags &= ~O_NONBLOCK;
    fcntl(server_fd, F_SETFL, flags);

}

void CServer::EndConnection(void)
{
    close(new_socket);
}


