#ifndef __DHT11_H_
#define __DHT11_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


class DHT11
{
    private:

        int fd;

    public:

        DHT11(void);

        ~DHT11(void);

        float* ReadValues();
};


#endif