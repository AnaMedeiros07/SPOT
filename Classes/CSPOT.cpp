#include"CSPOT.h"
CSPOT::~CSPOT(){

}

CSPOT::CSPOT(){
    
}

bool CSPOT::ConfigureThreads(int)
{
    pthread_attr_setinherished(&thread_attr,PTHREAF_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&thread_attr,SCHED_FIFO);
    thread_param.shed_priority = thread_priority;
    pthread_attr_setschedparam(&thread_attr,&thread_param);
    
}
void InitSemaphores()
{
    sem_init(&SMotionSensor,0,0);
    sem_init(&SReadServer,0,0);
}
bool CSPOT::CreateThreads(void)
{
    int tUpdate = pthread_create(&tUpdateSystem,NULL,UpdateSystem,NULL);
    int tSmoke = pthread_create(&tSmoke,NULL,Smoke,NULL);
    int tNotification = pthread_create(&tNotification,NULL,Notification,NULL);
    int tReadApp = pthread_create(&tReadApp,NULL,ReadApp,NULL);

    if( tUpdate || tSmoke || tNotification || tReadApp )
    {
        cout << " ERROR in the thread creation "<<endl;
    }
}
int CSPOT::InitMultithereading(void){
   
}
bool CSPOT::ConfigureServer(void)
{
       /*Server Creation*/
    if(server.Creat_Socket())
    {
        server.Indentify_socket();
        return true;
    }

    return false;
        
}
bool CSPOT::ConfigureDatabase(void)
{
    /*Create Database and the Tables*/
    server.createDB(DATABASE):
    server.createTable(DATABASE);
}
void CSPOT::Motion(void)
{
    // enviar a notificação de movimento
}
void CSPOT::*Notification()
{
    //verificar qual a notificacao a enviar

}
void CSPOT::*ReadApp(void)
{
 //start a connection
 // execute read function
 // close connection
}
void CSPOT::*UpdateSystem(void)
{
    int LimH=0,LimT=0,LimS=0;
    /*============= Read From Message Queue ==================  */
    msgqread_id = mq_open(MSGQOBJ_NAME, O_RDWR); // open the message queue;
     if (msgqread_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }
    msgsz = mq_receive(msgqread_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }
    /*=============================================================*/
            
            /* Set Sensor Numbers */
    /*_______________________________________________*/
    TemperatureSensor.Set_Value(atof(msgsz[2]));
    HumididySensor.Set_Value(atof(msgsz[3]));
    SmokeSensor.Set_Value(atof(msgsz[1]));
    /*_________________________________________________*/

    
    

}
 