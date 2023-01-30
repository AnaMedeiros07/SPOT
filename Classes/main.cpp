#include "CSensor.h"
#include "CDht11.h"
#include "CMq2.h"
#include <stdio.h>
#include "UPSHat.h"

int main()
{
    CMq2 mq2;
    //UPSHat ups;
    CDht11 dht11;

    dht11.ReadSensor();

    //printf("%f \n", ups.get_percentage());
    printf("%d \n", mq2.getStatus());
    printf("%f \n", dht11.GetTemperature());
    printf("%f \n", dht11.GetHumidity());
    //printf("%f %d %f %f \n ",ups.get_percentage() , mq2.getStatus() , dht11.GetTemperature(), dht11.GetHumidity());

    return 0;
}