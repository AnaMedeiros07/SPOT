#ifndef __DHT11_H_
#define __DHT11_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

typedef struct DHT{
    unsigned char data[5];
    char temperature_int = 0;
    char temperature_dec = 0;
    char humidity_int = 0;
    char humidity_dec = 0;
} dht_t;

class CDht11
{
    private:

        int dht_file;
        dht_t sensor;
        float temperature = 0;
        float humidity = 0;

    public:

        CDht11(void);

        ~CDht11(void);

        int ReadSensor(void);

        float GetTemperature(void);

        float GetHumidity(void);
};


#endif