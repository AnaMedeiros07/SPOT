#ifndef __CHOUSE_H_
#define __CHOUSE_H_
#include "CSensor.h"
#include<iostream>
#include<vector>
#include<string>


using namespace std; 
class CHouse
{
    private:

        string name;
        string password;

    public:

        CHouse(string,string);

        ~CHouse(void);

        bool Check_Login(string, string);

        double Sensor_Status(CSensor);

        int Motion_Status(void);

        std::vector<double> Check_Limits(CSensor,CSensor,CSensor);

};


#endif
