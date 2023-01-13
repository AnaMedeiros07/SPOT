#include "sqlite3.h"
#include <stdio.h>
#include <iostream>

using namespace std;

static int createDB(const char* s);
static int createTable(const char* s);
static int createUserEntryTable(const char* s);
static int deleteEventData(const char* s);
static int deleteUserData(const char* s);
static int deleteUserEntryData(const char* s);
static int deleteSensorData(const char* s);
static int deleteHouseData(const char* s);
static int insertDataEventID(const char* s,string sensorID,string houseID,string eventdata, string eventtime,string description);
static int insertDataSensorID(const char* s,string houseID,string type,string value,string upper_limit,string lower_limit);
static int insertDataHouseID(const char* s,string address);
static int insertDataUserID(const char* s,string name,string password,string number);
static int insertDataUserEntryID(const char* s,string houseid,string userid);
static int updateUserNumber(const char* s,string name, string number);
static int updateSensorValue(const char* s,string type,string value);
static int updateSensorUpperLimit(const char* s,string type,string upper_limit);
static int updateSensorUpperLimit(const char* s,string type,string upper_limit);
static bool checklogin(const char* s,string name,string password);
string quotesql(const string& word);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static char* FindValueCallback(void* NotUsed, int argc, char** argv, char** azColName);

int main()
{
	const char* DATABASE = "DATABASE.db";

	/*-----------create tables*------------------*/
	createDB(DATABASE);
	createTable(DATABASE);

	/*----------------------------------------------*/
	
	//deleteEventData(DATABASE); // use prior to insert
	insertDataEventID(DATABASE,"temperatura","2","10/01/2023","14:26","teste"); 
	insertDataSensorID(DATABASE,"temperature","2","30.0","20","40");
	insertDataUserID(DATABASE,"ggg","1234","915554441");
	insertDataHouseID(DATABASE,"5");
	updateUserNumber(DATABASE,"Ana","919666");
	//updateData(dir);
	checklogin(DATABASE,"Ana","1234");

	return 0;
}
//-------------- Auxilar function to insert values-------------
string quotesql( const string& word ) {
    return string("'") + word + string("'");
}

static int createDB(const char* s)
{
	sqlite3* DB;
	
	int exit = 0;
	exit = sqlite3_open(s, &DB);

	sqlite3_close(DB);

	return 0;
}




static int createTable(const char* s)
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

static int insertDataEventID(const char* s,string sensorID,string houseID,string eventdata, string eventtime,string description)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO EVENT (SENSORID ,EVENTDATE,EVENTTIME,DESCRIPTION) VALUES ("
    + quotesql(sensorID) + ","
    + quotesql(eventdata) + ","
    + quotesql(eventtime) + ","
	+ quotesql(description) + ");");

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
static int insertDataSensorID(const char* s,string type,string houseID, string value,string upper_limit,string lower_limit)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO SENSOR (HOUSEID ,TYPE, VALUE, UPPER_LIMIT, LOWER_LIMIT) VALUES ("
    + quotesql(houseID) + ","
    + quotesql(value) + ","
    + quotesql(upper_limit) + ","
	+ quotesql(lower_limit) + ");");

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
static int insertDataHouseID(const char* s,string address)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO HOUSE (ADDRESS) VALUES ("
	+ quotesql(address) + ");");

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
static int insertDataUserID(const char* s,string name,string password,string number)
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
static int insertDataUserEntryID(const char* s,string houseid,string userid)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO USERENTRY (HOUSEID, USERID) VALUES ("
    + quotesql(houseid) + ","
	+ quotesql(userid) + ");");

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
static int updateUserNumber(const char* s,string name, string number)
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
static int updateSensorValue(const char* s,string type,string value)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET VALUE = " + quotesql(value) +" WHERE TYPE = " + quotesql(type)+"");

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
static int updateSensorUpperLimit(const char* s,string type,string upper_limit){
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET UPPER_LIMIT = " + quotesql(upper_limit) +" WHERE TYPE = " + quotesql(type)+"");

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
static int updateSensorLowerLimit(const char* s,string type,string lower_limit){
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET LOWER_LIMIT = " + quotesql(lower_limit) +" WHERE TYPE = " + quotesql(type)+"");

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
static int deleteEventData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM EVENT;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return 0;
}
static int deleteSensorData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM SENSOR;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return 0;
}
static int deleteHouseData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM HOUSE;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return 0;
}
static int deleteUserData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM USER;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return 0;
}
static int deleteUserEntryData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "DELETE FROM USERENTRY;";

	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in deleteData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records deleted Successfully!" << endl;

	return 0;
}
static bool checklogin(const char* s,string name,string password)
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

// retrieve contents of database used by selectData()
/* argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array, */
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << endl;

	return 0;
}

