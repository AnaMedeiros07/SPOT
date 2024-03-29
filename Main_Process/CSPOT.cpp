#include "CSPOT.h"

sem_t CSPOT::SMotionSensor;
sem_t CSPOT::SNotification;

pthread_mutex_t CSPOT::sensor_resources = PTHREAD_MUTEX_INITIALIZER;

CSensor CSPOT::TemperatureSensor;
CSensor CSPOT::HumiditySensor;
CSensor CSPOT::SmokeSensor;

CDatabase CSPOT::Database(DATABASE);

CSPOT::~CSPOT(){
    sem_destroy(&SMotionSensor);
    sem_destroy(&SNotification);
}

CSPOT::CSPOT()
{
    TemperatureSensor.Change_Limits(40.0,10.0);
    HumiditySensor.Change_Limits(40.0,20.0);
}


// Read sensor values provided by Daemon through msg queue
int CSPOT::ReceiveMsg(string* Values)
{
    char msgcontent[MAX_MSG_LEN];
    unsigned int sender;
    int i = 0;
    string word;
    mqd_t msgqread_id = mq_open(MSGQOBJ_NAME, O_RDWR); // open the message queue;
    if (msgqread_id == (mqd_t)-1) {
        printf("%i", msgqread_id);
        perror("In Receive Msg mq_open()");
        exit(1);
    }
    int msgsz = mq_receive(msgqread_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }

    istringstream ss(msgcontent);

    while (ss >> word)
    {
        Values[i]=word;
        i++;
    }
    mq_close(msgqread_id);
    return 0;
}


//Read messages sent from server through msg queue
int CSPOT::ReceiveServerMsg(char* msg)
{
    char msgcontent [MAX_MSG_LEN] = {0};
    unsigned int sender;
    int i = 0;
    string word;
    mq_attr msgq_attr;
    mqd_t msgqread_id = mq_open(MSGQSEND_NAME, O_RDWR); // open the message queue;
    if (msgqread_id == (mqd_t)-1) {
        printf("%i", msgqread_id);
        perror("In Receive Server mq_open()");
        exit(1);
    }
    int msgsz = mq_receive(msgqread_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);    
    }
    strcpy(msg, msgcontent);
    mq_close(msgqread_id);
    return 0;
}

int CSPOT::CheckReceivequeue()
{
    mq_attr msgq_attr;
    mqd_t msgqread_id = mq_open(MSGQSEND_NAME, O_RDWR); // open the message queue;
    mq_getattr(msgqread_id, &msgq_attr);
    if(msgq_attr.mq_curmsgs == 0)
    { 
        mq_close(msgqread_id);
        return 0;
    }
    mq_close(msgqread_id);
    return 1;
}

//Send to server through msg queue
int CSPOT::SendServerMsg(char* msg)
{
    mqd_t msgq_id;
    unsigned int msgprio = 1;
    msgq_id = mq_open(MSGQRECEIVE_NAME, O_RDWR);
    if(msgq_id == (mqd_t)-1)
    {
        printf("%i", msgq_id);
        perror("In Send Server mq_open()");
        exit(1);
    }
    mq_send(msgq_id, msg, strlen(msg)+1, msgprio);
    mq_close(msgq_id);
    return 0;
}

void CSPOT::InitSemaphores()
{
    sem_init(&SMotionSensor,0,0);
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
    pthread_attr_setschedpolicy(&Motion_attr, SCHED_RR);
    pthread_attr_setdetachstate(&Motion_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (& Motion_attr ,&Motion_sched ) ;
    pthread_attr_init (&Motion_attr );

    // Set Update System Thread Priority
    Update_sched.sched_priority=3;
    pthread_attr_setschedpolicy(&Update_attr, SCHED_RR);
    pthread_attr_setdetachstate(&Update_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (&Update_attr ,&Update_sched ) ;
    pthread_attr_init (&Update_attr );

    // Set Notification System Thread Priority
    Notification_sched.sched_priority= 2;
    pthread_attr_setschedpolicy(&Notification_attr, SCHED_RR);
    pthread_attr_setdetachstate(&Notification_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (&Notification_attr ,&Notification_sched ) ;
    pthread_attr_init (&Notification_attr );
    
    // Set ReadApp System Thread Priority
    ReadApp_sched.sched_priority=3;
    pthread_attr_setschedpolicy(&ReadApp_attr, SCHED_RR);
    pthread_attr_setdetachstate(&ReadApp_attr,PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam (&ReadApp_attr ,&ReadApp_sched ) ;
    pthread_attr_init (&ReadApp_attr );

    int res = 0;
    res = pthread_create(&tUpdateSystem,&Update_attr,UpdateSystem,this);
    res = pthread_create(&tMotion,&Motion_attr,Motion,this);
    res = pthread_create(&tNotification,&Notification_attr,Notification,this);
    res = pthread_create(&tReadApp,&ReadApp_attr,ReadApp,this);

    if( res)
    {
        cout << " ERROR in the thread creation "<<endl;
        return false;
    }
    return true;
}

bool CSPOT::ConfigureDatabase(void)
{
    /*Create Database and the Tables*/
    Database.createTable(DATABASE);

    Database.insertDataSensorID("temperature","0","30.0","20.0");
    Database.insertDataSensorID("humidity","0","40.0","20.0");
    Database.insertDataSensorID("battery","0","0","0");
    Database.insertDataSensorID("motion","0","0","0");
    Database.insertDataSensorID("smoke","1","0","0");
    return true;
}

void* CSPOT::Motion(void* threadid)
{
    char msg[30] = "ALARM Motion Detected";
    while(1)
    {
        sem_wait(&SMotionSensor);
        cout << " Motion Detected!!"<<endl;
        SendServerMsg(msg);
    }
}

void* CSPOT::Notification(void* threadid)
{
    
    char msg[200];
    int flag = 0;
    string event;
    static string last_event;
    while(1)
    {
        sem_wait(&SNotification);

        pthread_mutex_lock(&sensor_resources);
        
        if(Database.CheckAllSensorLimits("temperature") == 1)
        {
            event = event + "Temperature Bellow ";
            flag = 1;
        }  
        else if(Database.CheckAllSensorLimits("temperature") == 2)
        {
            event = event + "Temperature Above ";
            flag = 1;
        } 
        if(Database.CheckAllSensorLimits("humidity") == 1)
        {
            event = event + "Humidity Bellow ";
            flag = 1;
        } 
        else if(Database.CheckAllSensorLimits("humidity") == 2)
        {
            event = event + "Humidity Above ";
            flag = 1;
        } 
        if(Database.GetSensorValues("smoke") == "0")
        {
            event = event + "Smoke Detected";
            flag = 1;
        }
        if(flag)
        {
            event = "ALARM " + event;   
            flag = 0;
        }
        pthread_mutex_unlock(&sensor_resources);
        
        if( (event != last_event) && (!event.empty()) )
        {
            cout << event << "\n";
            last_event = event;
            strcpy(msg, event.c_str());
            SendServerMsg(msg);
        }
        
        event.clear();
    }

}
void* CSPOT::ReadApp(void* threadid)
{
    char msg[MAX_MSG_LEN];
    string database_response;
    while(1)
    {

        ReceiveServerMsg(msg);
        pthread_mutex_lock(&sensor_resources);
        database_response = Database.ProcessRequest(msg);
        pthread_mutex_unlock(&sensor_resources);
        strcpy(msg, database_response.c_str());
        SendServerMsg(msg);
        memset(msg,0,MAX_MSG_LEN);
    }
    
}
void* CSPOT::UpdateSystem(void* threadid)
{
    int LimH=0,LimT=0;
    string Values[4];
    while(1)
    {
        //cout << " Thread Update System !"<<endl;
        /*============= Read From Message Queue ==================  */
        ReceiveMsg(Values);
        /*=============================================================*/
                /* Set Sensor Numbers */
        /*_______________________________________________*/
        pthread_mutex_lock(&sensor_resources);
        Database.updateUserSensor(Values[3],"temperature");
        Database.updateUserSensor(Values[2],"humidity");
        Database.updateUserSensor(Values[1],"smoke");
        Database.updateUserSensor(Values[0],"battery");

        /*_________________________________________________*/

        if(Database.CheckAllSensorLimits("temperature") || Database.CheckAllSensorLimits("humidity")
            || (Database.GetSensorValues("smoke") == "0"))
        {
            sem_post(&SNotification);
        }
        pthread_mutex_unlock(&sensor_resources);    

    }
}
 
void CSPOT::sig_event_handler(int n, siginfo_t* info, void* unused)
{
    if(n == SIGETX)
    {
        sem_post(&SMotionSensor);
    }
}
