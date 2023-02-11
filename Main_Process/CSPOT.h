#ifndef __CSPOT_H_
#define __CSPOT_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <iostream>
#include <signal.h>
#include <semaphore.h>
#include <mqueue.h>
#include <bits/stdc++.h>

#include "CSensor.h"
#include "CCamera.h"
#include "CHb100.h"
#include "CDatabase.h"

#define SIGETX 44

#define MSGQOBJ_NAME    "/myqueue" // message that have the sensor values

#define MSGQRECEIVE_NAME    "/receivequeue"
#define MSGQSEND_NAME       "/sendqueue"

#define MAX_MSG_LEN     10000

#define DATABASE "DATABASE.db"

using namespace std; 

class CSPOT{
	private:
	
        //Member Objects

        static CSensor TemperatureSensor, HumiditySensor,SmokeSensor;
        CHb100 MotionSensor;
        static CDatabase Database;

        //Threading Elements

        pthread_t tUpdateSystem, tNotification,tMotion,tSensors,tReadApp; 
        pthread_attr_t Motion_attr,Update_attr,Notification_attr, ReadApp_attr;
        struct sched_param Motion_sched,Update_sched,Notification_sched,ReadApp_sched;
        static sem_t SMotionSensor,SNotification;
        static pthread_mutex_t sensor_resources;

        mqd_t msgqread_id;

        string status;
        timer_t ISR_Receive_Data,ISR_Update_System;

        public:
        CSPOT(void);
        ~CSPOT(void);
        void InitSemaphores(void);
        bool CreateThreads(void);
        void InitSignal(void);
        void static sig_event_handler(int, siginfo_t*, void*);
        bool ConfigureDatabase(void);
        static int ReceiveMsg(string*);
        static int ReceiveServerMsg(char*);
        static int CheckReceivequeue();
        static int SendServerMsg(char*);
        static void *ReadApp(void*);
        static void *UpdateSystem(void*);
        static void *Motion(void*);
        static void *Notification(void*);

};
#endif
