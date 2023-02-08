#include "ServerMSGQ.h"

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

int CheckSendqueue(void)
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