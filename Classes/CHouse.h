#ifndef __CHOUSE_H_
#define __CHOUSE_H_

#include <string.h>
#include <stdio.h>
#include <iostream>

using std::string;

class CHouse
{
    private:

        string name;
        string password;

    public:

        CHouse(void);

        ~CHouse(void);

        int Check_Login(string, string);

        int Sensor_Status(void);

        int Motion_Status(void);

        int Check_Limits(void);

};


#endif