#include"CDht11.h"

CDht11::CDht11()
{
    printf("\nOpening Driver Dht\n");
    dht_file = open("/dev/dht_device", O_RDWR);
    if(dht_file < 0) {
            printf("Cannot open device file...\n");
    }
}

CDht11::~CDht11()
{
    close(dht_file);
}

int CDht11::ReadSensor(void)
{
    if(!read(dht_file, &sensor, sizeof(sensor)))
        return 1;

    temperature = sensor.temperature_int + sensor.temperature_dec/10.0;
    humidity = sensor.humidity_int + sensor.humidity_dec/10.0;
    
    return 0;
}

float CDht11::GetTemperature(void)
{
    return temperature;
}

float CDht11::GetHumidity(void)
{
    return humidity;
}