#include "MQ2.h"

MQ2::MQ2()
{
    printf("\nOpening Driver\n");
    mq2_file = open("/dev/mq2_device", O_RDWR);
    if(mq2_file < 0) {
            printf("Cannot open device file...\n");
    }
}

MQ2::~MQ2(void)
{

}

int MQ2::getStatus(void)
{

}