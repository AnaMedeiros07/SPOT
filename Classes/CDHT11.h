#ifndef __DHT11_H_
#define __DHT11_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define mem_size 5

class CDHT11
{
    private:

        int dht_file;
        char dht_read_buf[mem_size];
        float temperature;
        float humidity;

    public:

        CDHT11(void);

        ~CDHT11(void);

        int ReadSensor(void);

        float GetTemperature(void);

        float GetHumidity(void);
};


#endif