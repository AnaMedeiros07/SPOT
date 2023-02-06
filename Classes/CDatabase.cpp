#include "CDatabase.h"

string CDatabase::ProcessRequest(string str){
	istringstream ss(str);
    string word; // for storing each word
	int check;
	string result;
	string temperature;
	string humidity;
	string smoke;
	string motion;
	string battery;
	string upper_limitT;
	string lower_limitT;
	string upper_limitH;
	string lower_limitH;
    string argv[50];
    int i=0;
    while (ss >> word)
    {
        argv[i]=word;
        i++;
    }
    if(argv[0] =="RG"){
		check = CDatabase::insertDataUserID(argv[1],argv[2],argv[3]);
			if(check==0)
				return "T";
			else
				return "F";
	}
	else if(argv[0] =="LG"){
		check = CDatabase::checklogin(argv[1],argv[2]);
		if(check == 0)
			return "T";
		else
			return "F";
	}
	else if(argv[0] =="CNumber"){
		result = CDatabase::GetNumber(argv[1]);
		return result;
		}
				
	else if(argv[0] == "NNumber"){
		check = CDatabase::updateUserNumber(argv[1],argv[2]);
		if(check == 0)
			return "T";
		else
			return "F";
	}
	else if(argv[0] =="Values")
	{
		temperature = CDatabase::GetSensorValues("temperature");
		humidity    = CDatabase::GetSensorValues("humidity");
		smoke       = CDatabase::GetSensorValues("smoke");
		motion		= CDatabase::GetSensorValues("motion");
		battery		= CDatabase::GetSensorValues("battery");

		result = temperature + " " + humidity + " " + smoke + " " + motion + " " + battery;
		return result;
	}
	else if(argv[0] == "RGLimits")
	{
		check = CDatabase::updateUpperLimits("temperature",argv[1]);
		check = CDatabase::updateLowerLimits("temperature",argv[3]);
		check = CDatabase::updateUpperLimits("humidity",argv[2]);
		check = CDatabase::updateLowerLimits("humidity",argv[4]);
			if(check == 0)
				return "T";
			else
				return "F";
	}	
	else if(argv[0] == "Limits"){
		upper_limitT = CDatabase::GetUppperSensorLimits("temperature");
		upper_limitH = CDatabase::GetUppperSensorLimits("humidity");
		lower_limitT = CDatabase::GetLowerSensorLimits("temperature");
		lower_limitH = CDatabase::GetLowerSensorLimits("humidity");

		result = upper_limitT + " " + upper_limitH + " " + lower_limitT + " " + lower_limitH;
		return result;
	}
	else if(argv[0] == "NLimits")
	{
		check = CDatabase::updateUpperLimits(argv[1],argv[2]);
		check = CDatabase::updateLowerLimits(argv[1],argv[3]);
			if(check == 0)
				return "T";
			else
				return "F";
	}
	else		
		return "E";	
}
string CDatabase::quotesql( const string& word ) {
    return string("'") + word + string("'");
}

int CDatabase::createDB(const char* s)
{
	sqlite3* DB;
	
	int exit = 0;
	exit = sqlite3_open(s, &DB);

	sqlite3_close(DB);

	return 0;
}

  int CDatabase::createTable(const char* s)
{
	sqlite3 *DB;
	char* messageError;

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

 int CDatabase::insertDataUserID(string name,string password,string number)
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
 int CDatabase::insertDataSensorID(string type,string value,string upper_limit,string lower_limit)
{
	sqlite3* DB;
	char* messageError;
	
	string sql (
    "INSERT INTO SENSOR (TYPE, VALUE, UPPER_LIMIT,LOWER_LIMIT) VALUES ("
    + quotesql(type) + ","
    + quotesql(value) + ","
	+ quotesql(upper_limit) + ","
	+ quotesql(lower_limit) + ");");

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
int CDatabase::checklogin(string name,string password)
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

	return valid;
}
int CDatabase::updateUserNumber(string name, string number)
{
	sqlite3* DB;
	char* messageError;

	string sql("UPDATE USER SET NUMBER = " + quotesql(number) +" WHERE NAME = " + quotesql(name)+";");

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

int CDatabase::updateUpperLimits(string type,string upper_limit)
{
	sqlite3* DB;
	char* messageError;
	string sql("UPDATE SENSOR SET UPPER_LIMIT = " + quotesql(upper_limit) + " WHERE TYPE ="+ quotesql(type)+";");


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
int CDatabase::updateLowerLimits(string type,string lower_limit)
{
	sqlite3* DB;
	char* messageError;
	string sql("UPDATE SENSOR SET LOWER_LIMIT = " + quotesql(lower_limit) + " WHERE TYPE ="+ quotesql(type)+";");


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
int CDatabase::updateUserSensor(string value,string type)
{
	sqlite3* DB;
	char* messageError;
	string sql("UPDATE SENSOR SET VALUE = " + quotesql(value) + " WHERE TYPE ="+ quotesql(type)+";");


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

string CDatabase::GetNumber(string name)
{
	sqlite3* DB;
	char* messageError;
	string number = {0};
	string sql = " SELECT NUMBER FROM USER WHERE NAME = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	
	return number;
}

string CDatabase::GetSensorValues(string name)
{
	sqlite3* DB;
	char* messageError;
	string number = {0};
	string sql = " SELECT VALUE FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	
	return number;
}

string CDatabase::GetUppperSensorLimits(string name)
{
	sqlite3* DB;
	char* messageError;
	string number = {0};
	string sql = " SELECT UPPER_LIMIT FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	
	return number;
}

string CDatabase::GetLowerSensorLimits(string name)
{
	sqlite3* DB;
	char* messageError;
	string number = {0};
	string sql = " SELECT LOWER_LIMIT FROM SENSOR WHERE TYPE = " + quotesql(name) + ";";
	int exit = sqlite3_open("DATABASE.db", &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_stmt *stmt;
	exit = sqlite3_prepare_v2(DB, sql.c_str(), -1,&stmt, NULL);

	while(sqlite3_step(stmt) ==SQLITE_ROW){
		number = (char*)sqlite3_column_text(stmt, 0);
	};

	sqlite3_finalize(stmt);

	
	return number;
}
