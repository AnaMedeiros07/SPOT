#ifndef __CSPOT_H_
#define __CSPOT_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "CSensor.h"
#include "CCamera.h"
#include "CHouse.h"
#include "CServer.h"
#include <cstring>
#include <unistd.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<iostream>
#include<signal.h>
#include<semaphore.h>

#define MSGQOBJ_NAME    "/myqueue" // message that have the sensor values

#define MSGQVALUES    "/msgvalues"

#define MAX_MSG_LEN     10000

#define DATABASE "DATABASE.db"

using namespace std; 

class CSPOT{
	private:
	string status;
        timer_t ISR_Receive_Data,ISR_Update_System;
	pthread_t tUpdateSystem, tNotification,tMotion,tSensors,tReadApp,tMotion; 
        pthread_attr_t thread_attr;
        struct sched_param thread_param;
        sem_t SMotionSensor,SReadServer;
        mqd_t msgqread_id, msgqNotification_id;
        CSensor TemperatureSensor, HumididySensor,SmokeSensor;
        CServer server;
	public:
        CSPOT(void);
        ~CSPOT(void);
        void InitSemaphores(void);
        bool ConfigureThreads(int);
        bool CreateThreads(void);
        bool ConfigureServer(void);
        bool ConfigureDatabase(void);
        int  InitMultithereading(void);
        void *ReadApp(void);
        void *UpdateSystem(void);
        void *Motion(void);
        void *Notification(void);

};
#endif
