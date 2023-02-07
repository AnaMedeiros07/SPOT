#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <mqueue.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "CServer.h"

#define MSGQRECEIVE_NAME    "/receivequeue"
#define MSGQSEND_NAME       "/sendqueue"

#define MAX_MSG_LEN 10000

#define PORT 8080

void send_messagequeue(char* msg)
{
    mqd_t msgq_id;
    unsigned int msgprio = 1;
    msgq_id = mq_open(MSGQSEND_NAME, O_RDWR);
    if(msgq_id == (mqd_t)-1)
    {
        perror("In mq_open()");
        exit(1);
    }
    mq_send(msgq_id, msg, strlen(msg)+1, msgprio);
    mq_close(msgq_id);
}

void receive_messagequeue(char* msg)
{
    char msgcontent[MAX_MSG_LEN];
    unsigned int sender;
    int i = 0;
    mqd_t msgqread_id = mq_open(MSGQRECEIVE_NAME, O_RDWR); // open the message queue;
    if (msgqread_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }
    int msgsz = mq_receive(msgqread_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }
    strcpy(msg, msgcontent);
    mq_close(msgqread_id);
}

int create_message_queue(void)
{
    mqd_t msgq_send_id, msgq_receive_id;
    unsigned int msgprio = 1;
    mq_attr send_attr, receive_attr;

    msgq_send_id = mq_open(MSGQSEND_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
    msgq_receive_id = mq_open(MSGQRECEIVE_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
    if(msgq_receive_id == (mqd_t)-1)
    {
        perror("In mq_open receive()");
        return 1;
    }
    if(msgq_send_id == (mqd_t)-1) 
    {
        perror("In mq_open send()");
        return 1;
    }
    //Certify that message queues are clean
    if(mq_getattr(msgq_send_id, &send_attr) == 0)
    {
        char msgcontent[MAX_MSG_LEN];
        unsigned int sender;
        while(send_attr.mq_curmsgs)
        {
            mq_receive(msgq_send_id, msgcontent, MAX_MSG_LEN, &sender);
            send_attr.mq_curmsgs--;
        }
    }
    if(mq_getattr(msgq_receive_id, &receive_attr) == 0)
    {
        char msgcontent[MAX_MSG_LEN];
        unsigned int sender;
        while(receive_attr.mq_curmsgs)
        {
            mq_receive(msgq_receive_id, msgcontent, MAX_MSG_LEN, &sender);
            receive_attr.mq_curmsgs--;
        }
    }
    mq_close(msgq_receive_id);
    mq_close(msgq_send_id);
    return 0;
}

int CheckSendqueue()
{
    mq_attr msgq_attr;
    mqd_t msgqread_id = mq_open(MSGQRECEIVE_NAME, O_RDWR); // open the message queue;
    mq_getattr(msgqread_id, &msgq_attr);
    printf("Check: %i \n", msgq_attr.mq_curmsgs);
    if(msgq_attr.mq_curmsgs == 0)
    { 
        mq_close(msgqread_id);
        return 0;
    }
    mq_close(msgqread_id);
    return 1;
}

int main(int argc, char *argv[])
{
    pid_t pid, sid;
    int len, fd;
    //const time_t timebuf;

    CServer server;  

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

    // Creating socket file descriptor
    server.Create_Socket();
    server.Identify_socket();
   
    //Message Queue
    char send_msg[MAX_MSG_LEN] = {0};
    char receive_msg[MAX_MSG_LEN] = {0};

    create_message_queue();
    server.ConnectServer();
    while (1)
    {
        //Connect to server 
        
        //Receive and send data to main process
        if(CheckSendqueue())
        {
            receive_messagequeue(receive_msg);
            printf("On send %s \n", receive_msg);
            server.SendData(receive_msg);
        }
        server.ReceiveData(send_msg, MAX_MSG_LEN);
        send_messagequeue(send_msg);
        sleep(1);

        //Clear msg
        memset(send_msg,0,MAX_MSG_LEN);
        memset(receive_msg,0,MAX_MSG_LEN);
        //sleep(1);
    }
    server.EndConnection();
    printf("Ended Server");
    exit(EXIT_SUCCESS);
}
