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
        string CheckUpperLimits(string )
        string ProcessRequest(string);
      
};

#endif