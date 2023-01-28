#include"CDHT11.h"

CDHT11::CDHT11()
{
    printf("\nOpening Driver\n");
    dht_file = open("/dev/dht_device", O_RDWR);
    if(dht_file < 0) {
            printf("Cannot open device file...\n");
    }
}

CDHT11::~CDHT11()
{
    close(dht_file);
}

int CDHT11::ReadSensor(void)
{
    if(!read(dht_file, dht_read_buf, 256))
        return 1;

    printf("Read Buffer: %s\n",dht_read_buf);

    if(!strcmp(dht_read_buf,"Checksum failed"))
    {
        printf("Read Failed\n");
        return 1;
    }

    char* s = strtok(dht_read_buf, " ");

    temperature = (float)atof(s);

    printf("Temperature: %f \n", temperature);

    s = strtok(NULL, " ");

    humidity = (float)atof(s);

    printf("Humidity: %f \n", humidity);

    return 0;
}

float CDHT11::GetTemperature(void)
{
    printf("Temperature: %f \n", temperature);
    return temperature;
}

float CDHT11::GetHumidity(void)
{
    printf("Humidity: %f \n", humidity);
    return humidity;
}