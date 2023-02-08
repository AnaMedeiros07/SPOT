#include "CDatabase.h"

CDatabase::CDatabase(const char* s)
{	
	sqlite3_open(s, &DB);
}
CDatabase::~CDatabase()
{
	sqlite3_close(DB);
}

string CDatabase::ProcessRequest(string str){
	istringstream ss(str);
    string word; // for storing each word
	int check;
	string result,temperature,humidity,smoke, motion, battery, upper_limitT, lower_limitT, upper_limitH, lower_limitH;
    string argv[50];
    int i=0;
    while (ss >> word) // split the string in spaces
    {
        argv[i]=word;
        i++;
    }
    if(argv[0] =="RG"){ // Register ( Add new user)
		check = CDatabase::insertDataUserID(argv[1],argv[2],argv[3]);
			if(check==0)
				return "RegisterT";
			else
				return "RegisterF";
	}
	else if(argv[0] =="LG"){ //Login 
		check = CDatabase::checklogin(argv[1],argv[2]);
		if(check == 0)
			return "LoginT";
		else
			return "LoginF";
	}
	else if(argv[0] =="CNumber"){ //Current Mobile Number
		result = CDatabase::GetNumber(argv[1]);
		result = "CNumber " + result;
		return result;
		}
				
	else if(argv[0] == "NNumber"){ // New Mobile Number
		check = CDatabase::updateUserNumber(argv[1],argv[2]);
		if(check == 0)
			return "NNumberT";
		else
			return "NNumberF";
	}
	else if(argv[0] =="Values") // Values of the sensores 
	{
		temperature = CDatabase::GetSensorValues("temperature");
		humidity    = CDatabase::GetSensorValues("humidity");
		smoke       = CDatabase::GetSensorValues("smoke");
		motion		= CDatabase::GetSensorValues("motion");
		battery		= CDatabase::GetSensorValues("battery");

		result = "Values " + temperature + " " + humidity + " " + smoke + " " + motion + " " + battery;
		return result;
	}
	else if(argv[0] == "RGLimits") // Configuration of the Limits after register 
	{
		check = CDatabase::updateUpperLimits("temperature",argv[1]);
		check = CDatabase::updateLowerLimits("temperature",argv[3]);
		check = CDatabase::updateUpperLimits("humidity",argv[2]);
		check = CDatabase::updateLowerLimits("humidity",argv[4]);
			if(check == 0)
				return "RGLimitsT";
			else
				return "RGLimitsF";
	}	
	else if(argv[0] == "Limits"){ // Send current limits
		upper_limitT = CDatabase::GetUppperSensorLimits("temperature");
		upper_limitH = CDatabase::GetUppperSensorLimits("humidity");
		lower_limitT = CDatabase::GetLowerSensorLimits("temperature");
		lower_limitH = CDatabase::GetLowerSensorLimits("humidity");

		result = "Limits " + upper_limitT + " " + upper_limitH + " " + lower_limitT + " " + lower_limitH;
		return result;
	}
	else if(argv[0] == "NLimits") // Update Limits
	{
		check = CDatabase::updateUpperLimits(argv[1],argv[2]);
		check = CDatabase::updateLowerLimits(argv[1],argv[3]);
			if(check == 0)
				return "NLimitsT";
			else
				return "NLimitsF";
	}
	else		
		return "";	
}
string CDatabase::quotesql( const string& word ) {
    return string("'") + word + string("'");
}

  int CDatabase::createTable(const char* s)
{
	string sql = 
		//___________Create Sensor table_______________
		"CREATE TABLE IF NOT EXISTS SENSOR("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TYPE TEXT NOT NULL UNIQUE,"
		"VALUE TEXT NOT NULL,"
		"UPPER_LIMIT  TEXT NOT NULL,"
		"LOWER_LIMIT TEXT NOT NULL);"
		//________________Create User table______________ 
		"CREATE TABLE IF NOT EXISTS USER("
		"ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
		"NAME TEXT NOT NULL UNIQUE, "
		"PASSWORD TEXT NOT NULL, "
		"NUMBER TEXT  NOT NULL );";
		
	try
	{
		int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}

 int CDatabase::insertDataUserID(string name,string password,string number)
{
	string sql (
    "INSERT OR IGNORE INTO USER (NAME, PASSWORD, NUMBER) VALUES ("
    + quotesql(name) + ","
    + quotesql(password) + ","
	+ quotesql(number) + ");");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	return 0;
}
 int CDatabase::insertDataSensorID(string type,string value,string upper_limit,string lower_limit)
{
	string sql (
    "INSERT OR REPLACE INTO SENSOR (TYPE, VALUE, UPPER_LIMIT,LOWER_LIMIT) VALUES ("
    + quotesql(type) + ","
    + quotesql(value) + ","
	+ quotesql(upper_limit) + ","
	+ quotesql(lower_limit) + ");");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}
	return 0;
}
int CDatabase::checklogin(string name,string password)
{
	int valid = 1;
	string sql = " SELECT ID FROM USER WHERE NAME = " + quotesql(name) + " AND PASSWORD = "  + quotesql(password)+";";

	sqlite3_stmt *stmt;
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		valid = 0;
	};

	sqlite3_finalize(stmt);

	return valid;
}
int CDatabase::updateUserNumber(string name, string number)
{
	string sql("UPDATE USER SET NUMBER = " + quotesql(number) +" WHERE NAME = " + quotesql(name)+";");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}

	return 0;
}

int CDatabase::updateUpperLimits(string type,string upper_limit)
{
	
	string sql("UPDATE SENSOR SET UPPER_LIMIT = " + quotesql(upper_limit) + " WHERE TYPE ="+ quotesql(type)+";");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}

	return 0;
}
int CDatabase::updateLowerLimits(string type,string lower_limit)
{
	
	string sql("UPDATE SENSOR SET LOWER_LIMIT = " + quotesql(lower_limit) + " WHERE TYPE ="+ quotesql(type)+";");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}

	return 0;
}
int CDatabase::updateUserSensor(string value,string type)
{

	string sql("UPDATE SENSOR set VALUE = " + quotesql(value) + " where TYPE ="+ quotesql(type)+";");

	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in updateData function." << endl;
		sqlite3_free(messageError);
		return 1;
	}

	return 0;
}

string CDatabase::GetNumber(string name)
{
	string number = {0};
	string sql = " SELECT NUMBER FROM USER WHERE NAME = " + quotesql(name) + ";";
	
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}

string CDatabase::GetSensorValues(string name)
{
	
	string number = {0};
	string sql = " SELECT VALUE FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}

string CDatabase::GetUppperSensorLimits(string name)
{

	string number = {0};
	string sql = " SELECT UPPER_LIMIT FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}

string CDatabase::GetLowerSensorLimits(string name)
{
	string number = {0};
	string sql = " SELECT LOWER_LIMIT FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}
string CDatabase::CheckLowerLimits(string name)
{
	
	string number = {0};
	string sql = " SELECT VALUE FROM SENSOR WHERE TYPE = " + quotesql(name) + "AND LOWER_LIMIT > VALUE ;";
	
	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}
string CDatabase::CheckUpperLimits(string name)
{
	
	string number = {0};
	string sql = " SELECT VALUE FROM SENSOR WHERE TYPE = " + quotesql(name) + "AND UPPER_LIMIT < VALUE ;";

	int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	return number;
}
int CDatabase::CheckAllSensorLimits(string name)
{
	int ret = 0;

	// returns 1 if value of the sensor<lowerlimit
	if(CheckLowerLimits(name).length()-1)  
		ret = 1;

	// returns 2 if value of the sensor>upperlimit
	if(CheckUpperLimits(name).length()-1)
		ret = 2; 

	return ret;
}