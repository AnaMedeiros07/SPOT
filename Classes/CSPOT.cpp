#include "CSPOT.h"

sem_t CSPOT::SMotionSensor;
sem_t CSPOT::SReadServer;
sem_t CSPOT::SNotification;

CSPOT::~CSPOT(){
    sem_destroy(&SMotionSensor);
    sem_destroy(&SReadServer);
    sem_destroy(&SNotification);
}

CSPOT::CSPOT(){
    
}
int CSPOT::ReceiveMsg(string* Values)
{
    char msgcontent[MAX_MSG_LEN];
    unsigned int sender;
    int i = 0;
    string result[10];
    string word;
    mqd_t msgqread_id = mq_open(MSGQOBJ_NAME, O_RDWR); // open the message queue;
    if (msgqread_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }
    int msgsz = mq_receive(msgqread_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }

    istringstream ss(msgcontent);

    std::cout << msgcontent << "\n";

    while (ss >> word)
    {
        Values[i]=word;
        i++;
        std::cout << word << "\n";
    }

    return 0;
    
}
void CSPOT::InitSemaphores()
{
    sem_init(&SMotionSensor,0,0);
    sem_init(&SReadServer,0,0);
    sem_init(&SNotification,0,0);
}
void CSPOT::InitSignal()
{
    MotionSensor.CreateSignalHandler(sig_event_handler);
}

bool CSPOT::CreateThreads(void)
{
    // Set Motion Thread Priority
    Motion_sched.sched_priority=1;
    pthread_attr_setdetachstate(&Motion_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (& Motion_attr ,& Motion_sched ) ;
    pthread_attr_init (& Motion_attr ) ;

    // Set Update System Thread Priority
    Update_sched.sched_priority=3;
    pthread_attr_setdetachstate(&Update_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (&Update_attr ,& Update_sched ) ;
    pthread_attr_init (&Update_attr ) ;

    // Set Notification System Thread Priority
    Update_sched.sched_priority= 2;
    pthread_attr_setdetachstate(&Notification_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (&Notification_attr ,&Notification_sched ) ;
    pthread_attr_init (&Notification_attr ) ;
    
    int res = 0;
    res = pthread_create(&tUpdateSystem,&Update_attr,UpdateSystem,this);
    //int tSmoke = pthread_create(&tSmoke,NULL,Smoke,NULL);
    res = pthread_create(&tMotion,&Motion_attr,Motion,this);
    res = pthread_create(&tNotification,&Notification_attr,Notification,this);
   // int tReadApp = pthread_create(&tReadApp,NULL,ReadApp,NULL);

    if( res)
    {
        cout << " ERROR in the thread creation "<<endl;
        return false;
    }
    return true;
}

bool CSPOT::ConfigureServer(void)
{
    CServer server;
       /*Server Creation*/
    if(server.Creat_Socket())
    {
        server.Identify_socket();
        return true;
    }

    return false;
        
}
bool CSPOT::ConfigureDatabase(void)
{
    /*Create Database and the Tables*/
    CServer server;
    server.createDB(DATABASE);
    server.createTable(DATABASE);
}
void* CSPOT::Motion(void* threadid)
{
    while(1)
    {
        sem_wait(&SMotionSensor);
        cout << " Motion Detected!!"<<endl;
    }
}
void* CSPOT::Notification(void* threadid)
{
    while(1)
    {
        printf("Smoke Detected\n");
        sem_wait(&SNotification);
        
    }

}
void* CSPOT::ReadApp(void* threadid)
{
 //start a connection
 // execute read function
 // close connection
}
void* CSPOT::UpdateSystem(void* threadid)
{
    int LimH=0,LimT=0,LimS=0;
    string Values[4];
    CSensor TemperatureSensor, HumididySensor,SmokeSensor;
    while(1)
    {
        cout << " Thread Update System !"<<endl;
        /*============= Read From Message Queue ==================  */
        ReceiveMsg(Values);
        /*=============================================================*/
                /* Set Sensor Numbers */
        /*_______________________________________________*/
        TemperatureSensor.Set_Value(stof(Values[3]));
        HumididySensor.Set_Value(stof(Values[2]));
        SmokeSensor.Set_Value(stof(Values[1]));
        /*_________________________________________________*/
        if(Values[1]== "0"){
            sem_post(&SNotification);
            
        }
    }
}
 
void CSPOT::sig_event_handler(int n, siginfo_t* info, void* unused)
{
    if(n == SIGETX)
    {
        sem_post(&SMotionSensor);
    }
}
