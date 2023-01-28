#include "CSensor.h"
#include "CDHT11.h"
#include <stdio.h>

int main()
{
    CDHT11 dht11;

    dht11.ReadSensor();

    dht11.GetTemperature();

    dht11.GetHumidity();

    return 0;
}