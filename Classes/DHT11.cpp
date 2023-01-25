#include"DHT11.h"


DHT11::DHT11()
{
     printf("\nOpening Driver\n");
    fd = open("/dev/dht_device", O_RDWR);
    if(fd < 0) {
            printf("Cannot open device file...\n");
    }
}

DHT11::~DHT11()
{
    
}

float* DHT11::ReadValues()
{
    float* values[2];

    read(fd,values,sizeof(float)*2);

    return values;
}