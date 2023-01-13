
#include<iostream>
#include<string.h>
#include"CServer.h"
using std::string;

#define PORT 8080
#define UCMD_DEFAULT_DELIMETER " "

using namespace std;

CServer::CServer(void){
    
}

CServer::~CServer(void){

}

bool CServer::Creat_Socket(void)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
     if (this->server_fd == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
        return false;
    }
    int flags = fcntl(this->server_fd, F_GETFL);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
    return true;
}
void CServer::Indentify_socket(char* IP)
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
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

void CServer::ConnectServer(void)
{
     
    new_socket = accept(server_fd, NULL, NULL);
    if (new_socket == -1) {
        if (errno == EWOULDBLOCK) {
            printf("No pending connections; sleeping for one second.\n");
            sleep(1);
        } else {
            perror("error when accepting connection");
            exit(1);
        }
    }
}

void CServer::SendData(char* message)
{
    send(new_socket , message , strlen(message),0);
}
char* CServer::ReceiveData(void)
{
    timeout.tv_sec = 1; // Time out after a minute
    timeout.tv_usec = 0;
    fd_set read_set;
    char buffer[30000] = {0};
    FD_ZERO(&read_set);
    FD_SET(new_socket, &read_set);
    const char* DATABASE = "DATABASE.db";

    int r=select(new_socket+1, &read_set, NULL, NULL, &timeout);

    if( r<0 ) {
        // Handle the error
        cout<<"Error!!"<<endl;
    }

    if( r==0 ) {
        // Timeout - handle that. You could try waiting again, close the socket...
        //cout<<"timeout!!"<<endl;
    }

    if( r>0 ) {
        read(new_socket , buffer, 30000);
        printf("%s\n",buffer );
        if(!strcmp(buffer,"RG test 1234 919919222")) 
        {
            insertDataUserID(DATABASE,"test","1234","919919222");
            SendData("T");
        }
        if(!strcmp(buffer,"LG test 1234")) 
            {
                if(checklogin(DATABASE,"test","1234"))
                    SendData("T");
                else
                   SendData("F"); 
            }
        if(!strcmp(buffer,"NNumber Ana 919502245")) 
            {
                updateUserNumber(DATABASE,"Ana","919502245");
            }
        if(!strcmp(buffer,"CNumber Ana")) 
        {
            //SendData(GetNumber(DATABASE,"Ana"));
        }
        if(!strcmp(buffer,"Limits")) 
        {
            // Send Limits
        }

        // is missing the reception of new limits
    }
   
    return buffer;
}

void CServer::EndConnection(void)
{
    close(new_socket);
}
bool CServer::Updatelocalhouse(double, double, double, double){

}

bool CServer::ProcessRequest(string str){
    //UcmdParse(&my_cmd_list,"",str);
}

bool CServer::SendValuesOutOfRange(void){

}

bool CServer::SendImage(void){

}
string CServer::quotesql( const string& word ) {
    return string("'") + word + string("'");
}

int CServer::createDB(const char* s)
{
	sqlite3* DB;
	
	int exit = 0;
	exit = sqlite3_open(s, &DB);

	sqlite3_close(DB);

	return 0;
}

  int CServer::createTable(const char* s)
{
	sqlite3 *DB;
	char* messageError;

	string sql = 
		// _________Create House Table_________________
		"CREATE TABLE IF NOT EXISTS HOUSE("
		"ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
		"ADDRESS TEXT NOT NULL); "
		//___________Create Sensor table_______________
		"CREATE TABLE IF NOT EXISTS SENSOR("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"HOUSEID INTEGER NOT NULL REFERENCES HOUSE(ID) , "
		"TYPE TEXT NOT NULL UNIQUE,"
		"VALUE DOUBLE NOT NULL,"
		"UPPER_LIMIT  DOUBLE NOT NULL,"
		"LOWER_LIMIT DOUBLE NOT NULL);"
		//_______________Create Event table____________________
		"CREATE TABLE IF NOT EXISTS EVENT("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"SENSORID INTEGER NOT NULL REFERENCES SENSOR(TYPE) , "
		"EVENTDATE STRING  NOT NULL, "
		"EVENTTIME STRING NOT NULL, "
		"DESCRIPTION STRING NOT NULL );"
		//________________Create User table______________ 
		"CREATE TABLE IF NOT EXISTS USER("
		"ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
		"NAME TEXT NOT NULL UNIQUE, "
		"PASSWORD TEXT NOT NULL, "
		"NUMBER INT  NOT NULL );"
		//________________Create UserEntry table_________________
		"CREATE TABLE IF NOT EXISTS USERENTRY("
		"HOUSEID INTEGER NOT NULL,"
		"USERID INTEGER NOT NULL, "
		"FOREIGN KEY (HOUSEID) REFERENCES HOUSE(ID), "
		"FOREIGN KEY (USERID) REFERENCES USER(ID));";
		
	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}
 int CServer::insertDataUserID(const char* s,string name,string password,string number)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO USER (NAME, PASSWORD, NUMBER) VALUES ("
    + quotesql(name) + ","
    + quotesql(password) + ","
	+ quotesql(number) + ");");

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return 0;
}
bool CServer::checklogin(const char* s,string name,string password)
{
	sqlite3* DB;
	char* messageError;
	bool valid = false;
	string sql = " SELECT ID FROM USER WHERE NAME = " + quotesql(name) + " AND PASSWORD = "  + quotesql(password)+";";
	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		valid = true;
	};

	sqlite3_finalize(stmt);

	//cout <<valid;
	return valid;
}
int CServer::updateUserNumber(const char* s,string name, string number)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET NUMBER = " + quotesql(number) +" WHERE NAME = " + quotesql(name)+"");

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records updated Successfully!" << endl;

	return 0;
}
string CServer::GetNumber(const char* s,string name)
{
	sqlite3* DB;
	char* messageError;
	bool valid = false;
	string number = {0};
	string sql = " SELECT NUMBER FROM USER WHERE NAME = " + quotesql(name) + ";";
	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	//cout <<valid;
	return number;
}
