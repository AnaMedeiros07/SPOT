#ifndef __SERVERMSGQ_H__
#define __SERVERMSGQ_H__

#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSGQRECEIVE_NAME    "/receivequeue"
#define MSGQSEND_NAME       "/sendqueue"

#define MAX_MSG_LEN 10000

void send_messagequeue(char* msg);

void receive_messagequeue(char* msg);

int create_message_queue(void);

int CheckSendqueue(void);

int CheckNumMsg(void);

int CheckNumMsg2(void);
#endif