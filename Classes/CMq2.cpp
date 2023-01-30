#include "CMq2.h"

CMq2::CMq2()
{
    printf("\nOpening Driver\n");
    mq2_file = open("/dev/mq2_device", O_RDWR);
    if(mq2_file < 0) {
            printf("Cannot open device file...\n");
    }
}

CMq2::~CMq2(void)
{
    close(mq2_file);
}

int CMq2::ReadSensor(void)
{
    bool value = 0;

    if(!read(mq2_file, &value, 1) < 0)
        return 1;

    mq2_value = value;

    return 0;
}

int CMq2::getStatus(void)
{
    return mq2_value;
}