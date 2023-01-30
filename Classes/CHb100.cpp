#include "CHb100.h"



CHb100::CHb100(void)
{
    int32_t number;
    struct sigaction act;

    printf("\nOpening Driver\n");
    fd = open("/dev/hb100_device", O_RDWR);
    if(fd < 0) {
            printf("Cannot open device file...\n");
    }
 
    printf("Registering application ...");
    /* register this task with kernel for signal */
    if (ioctl(fd, REG_CURRENT_TASK,(int32_t*) &number)) {
        printf("Failed\n");
        close(fd);
        exit(1);
    }
    printf("Done!!!\n");
}

CHb100::~CHb100(void)
{
    close(fd);
}
