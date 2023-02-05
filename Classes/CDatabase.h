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

    public:
    //____________create tables_________________________
        int createDB(const char* );
        int createTable(const char* );
    //______________________________________________
    //_______________Insert in the database__________________
        int insertDataEventID(const char*,string ,string,string, string,string);
        int insertDataSensorID(const char* ,string ,string ,string ,string ,string );
        int insertDataHouseID(const char* ,string );
        int insertDataUserID(string ,string ,string );
        int insertDataUserEntryID(const char* ,string ,string );
    //___________________________________________________
    //______________Update Values in Database___________________
        int updateUserNumber(string , string );
        int updateUserSensor(string value,string type,string user );
        int updateUserLimits(string lower_limit,string upper_limit,string type,string user );
        char* GetNumber(string name);
        char* GetUserID(string name );
    //______________________Auxiliar Function_________________________________
        string quotesql(const string& );
        int checklogin(string name,string password);
        char* ProcessRequest(string);
};

#endif