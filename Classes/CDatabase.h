#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include <iostream>
#include <string>
#include <stdlib.h>
#include "sqlite3.h"
#include <bits/stdc++.h>

using namespace std;

class CDatabase
{
    private:
        sqlite3* DB;
        sqlite3_stmt *stmt;
	    char* messageError;
    public:
    //____________create tables_________________________
        CDatabase(const char*);
        ~CDatabase();
        int createTable(const char* );
    //______________________________________________
    //_______________Insert in the database__________________
        int insertDataSensorID(string ,string ,string ,string);
        int insertDataUserID(string ,string ,string );
    //___________________________________________________
    //______________Update and Get Values in Database___________________
        int updateUserNumber(string , string );
        int updateUserSensor(string, string);
        int updateLowerLimits(string, string);
        int updateUpperLimits(string, string);
        string GetNumber(string);
        string GetSensorValues(string);
        string GetUppperSensorLimits(string);
        string GetLowerSensorLimits(string);

    //______________________Auxiliar Function_________________________________
        string quotesql(const string& );
        int checklogin(string ,string );
        string CheckLowerLimits(string );  
        string CheckUpperLimits(string );
        int CheckAllSensorLimits(string )
        string ProcessRequest(string);
      
};

#endif