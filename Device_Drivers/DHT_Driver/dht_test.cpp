#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct DHT{
    unsigned char data[5];
    char temperature_int = 0;
    char temperature_dec = 0;
    char humidity_int = 0;
    char humidity_dec = 0; 
} dht_t;

int main(void)
{
    int file = open("/dev/dht_device",O_RDWR);
    if(file < 0)
        exit(1);

    dht_t sensor;

    int read_out = read(file, &sensor, sizeof(sensor));
    printf("%d \n", read_out);
    
    if(!read_out)
    {
        printf("Failed Read op");
        exit(1);
    }
    printf("Data %d.%d %d.%d %d\n", sensor.data[0], sensor.data[1], sensor.data[2], sensor.data[3], sensor.data[4]);
    
    printf("Final Data %d.%d %d.%d\n", sensor.temperature_int, sensor.temperature_dec, sensor.humidity_int, sensor.humidity_dec);
    
    float temperature = sensor.temperature_int + sensor.temperature_dec/10.0;
    
    return 0;  
}