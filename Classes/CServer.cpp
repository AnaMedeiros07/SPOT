#include <stdlib.h>
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
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
	return true;
}
void CServer::Identify_socket()
{
 	address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
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

int CServer::ConnectServer(void)
{
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
	return 0;
}

void CServer::SendData(char* message)
{

}

int CServer::CheckSocket(void)
{

}

void CServer::ReceiveData(char* msg, int msg_length)
{
    valread = read( new_socket , msg, msg_length);

}

void CServer::EndConnection(void)
{
    close(new_socket);
}

char* CServer::ProcessRequest(string str){

	istringstream ss(str);
    string word; // for storing each word
	int check;
	char* result;
    string argv[10];
    int i=0;
    while (ss >> word)
    {
        argv[i]=word;
        i++;
    }
    if(argv[0] =="RG"){
		check = CServer::insertDataUserID(argv[1],argv[2],argv[3]);
			if(check==0)
				return "T";
			else
				return "F";
	}
	if(argv[0] =="LG"){
		check = CServer::checklogin(argv[1],argv[2]);
		if(check == 0)
			return "T";
		else
			return "F";
	}
	if(argv[0] =="CNumber"){
		result = CServer::GetNumber(argv[1]);
		return result;
		}
				
	if(argv[0] == "NNumber"){
		check = CServer::updateUserNumber(argv[1],argv[2]);
		if(check == 0)
			return "T";
		else
			return "F";
	}
	if(argv[0] =="Values")
	{
		cout << "Send sensor values!!!!!";
	}
			
	if(argv[0] == "Limits")
		cout << "Send Limits!!"<<endl;

	if(argv[0] == "NLimits")
	{
		check = CServer::updateUserLimits(argv[1],argv[2],argv[3],argv[4]);
			if(check == 0)
				return "T";
			else
				return "F";
	}
			
	 return "E";
		
	
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
 int CServer::insertDataUserID(string name,string password,string number)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO USER (NAME, PASSWORD, NUMBER) VALUES ("
    + quotesql(name) + ","
    + quotesql(password) + ","
	+ quotesql(number) + ");");

	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
		cout << "Records inserted Successfully!" << endl;

	return 0;
}
int CServer::checklogin(string name,string password)
{
	sqlite3* DB;
	char* messageError;
	int valid = 1;
	string sql = " SELECT ID FROM USER WHERE NAME = " + quotesql(name) + " AND PASSWORD = "  + quotesql(password)+";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		valid = 0;
	};

	sqlite3_finalize(stmt);

	//cout <<valid;
	return valid;
}
int CServer::updateUserNumber(string name, string number)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET NUMBER = " + quotesql(number) +" WHERE NAME = " + quotesql(name)+"");

	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
		cout << "Records updated Successfully!" << endl;

	return 0;
}
char* CServer::GetUserID(string name )
{
	sqlite3* DB;
	char* messageError;
	bool valid = false;
	char* id;
	string sql = " SELECT ID FROM USER WHERE NAME = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		id = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	//cout <<valid;
	return id;
}
int CServer::updateUserLimits(string lower_limit,string upper_limit,string type,string user )
{
	sqlite3* DB;
	char* messageError;
	char* userid = CServer::GetUserID(user);
	string sql("UPDATE SENSOR SET LOWER_LIMIT = " + quotesql(lower_limit) + "AND SET UPPER LIMIT = " + quotesql(upper_limit) + " WHERE TYPE ="+ quotesql(type) +"AND HOUSEID ="+quotesql(userid)+"");


	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
		cout << "Records updated Successfully!" << endl;

	return 0;
}
int CServer::updateUserSensor(string value,string type,string user )
{
	sqlite3* DB;
	char* messageError;
	char* userid = CServer::GetUserID(user);
	string sql("UPDATE SENSOR SET VALUE = " + quotesql(value) + " WHERE TYPE ="+ quotesql(type) +"AND HOUSEID ="+quotesql(userid)+"");


	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	else
		cout << "Records updated Successfully!" << endl;

	return 0;
}
char* CServer::GetNumber(string name)
{
	sqlite3* DB;
	char* messageError;
	char* number;
	string sql = " SELECT NUMBER FROM USER WHERE NAME = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
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
