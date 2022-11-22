#ifndef __CHOUSE_H_
#define __CHOUSE_H_

class CHouse
{
    private:

        char* name;
        char* password;

    public:

        CHouse(void);

        ~CHouse(void);

        int Check_Login(char*, char*);

        int Sensor_Status(char*, char*);

        int Motion_Status(void);

        int Check_Limits(void);

};


#endif