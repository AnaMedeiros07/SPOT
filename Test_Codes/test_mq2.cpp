#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int file = open("/dev/mq2_device",O_RDWR);
    if(file < 0)
        exit(1);

    int value = 0;

    while (1)
    {
        if(!read(file, &value, 1) < 0)
        return 1;

        if(value)
            printf("No smoke detected\n");
        else
            printf("Smoke Detected\n");

        sleep(1);

    }

    return 0;

}