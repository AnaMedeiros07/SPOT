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
#include "UPSHat.h"
#include "CMq2.h"
#include "CDht11.h"
#include <pthread.h>
#include <mqueue.h>

#define MSGQOBJ_NAME "/myqueue"
#define MAX_MSG_LEN 80

void send_messagequeue(char* msg)
{
    mqd_t msgq_id;
    unsigned int msgprio = 1;
    msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
    if(msgq_id == (mqd_t)-1)
    {
        perror("In mq_open()");
        exit(1);
    }
    mq_send(msgq_id, msg, strlen(msg)+1, msgprio);
    mq_close(msgq_id);
}

int main(int argc, char *argv[])
{
    pid_t pid, sid;
    int len, fd;
    //const time_t timebuf;

    openlog("SPOT Daemon Test", LOG_PID, LOG_DAEMON);
    pid = fork(); // create a new process

    if (pid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        printf("Daemon PID: %d\n", pid);
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
    umask(0);             // read and write permission
    close(STDIN_FILENO);  // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor

    // Sensor Objects

    UPSHat charger;
    CMq2 mq2;
    CDht11 dht11;

    float percentage;
    float temperature;
    float humidity;
    bool motion;
    int smoke;

    //Message Queue
    char msg[MAX_MSG_LEN];



    while (1)
    {
        
        //percentage = charger.get_percentage();
        smoke = mq2.getStatus();
        dht11.ReadSensor();
        temperature = dht11.GetTemperature();
        humidity = dht11.GetHumidity();
        
        snprintf(msg, MAX_MSG_LEN, "%f %d %f %f", percentage, smoke, temperature, humidity);

        send_messagequeue(msg);
        // enviar valores por message queue

        sleep(0.5);
    }
    exit(EXIT_SUCCESS);
}
