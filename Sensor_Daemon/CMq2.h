#ifndef __CMQ2_H_
#define __CMQ2_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class CMq2
{
    private:

        int mq2_file;
        int mq2_value; //active on 0

    public:

        CMq2(void);
        
        ~CMq2(void);

        int ReadSensor(void);

        int getStatus(void);
};

#endif