#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
 
 
int main()
{
    int fd;
    int32_t value;

    printf("\nOpening Driver\n");
    fd = open("/dev/mq2_device", O_RDWR);
    if(fd < 0) {
            printf("Cannot open device file...\n");
            return 0;
    }
 
    read(fd, value, sizeof(value));
    printf("Value: %d",value);
    printf("Closing Driver\n");
    close(fd);
}