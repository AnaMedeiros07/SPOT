#include "CSensor.h"
#include "CDht11.h"
#include "CMq2.h"
#include <stdio.h>

int main()
{
    CMq2 mq2;

    printf("Value %d\n", mq2.getStatus());

    return 0;
}