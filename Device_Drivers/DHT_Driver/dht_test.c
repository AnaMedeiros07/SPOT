#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

int8_t dht_read_buf[256];
 
int main()
{
    int fd;
    int32_t value;

    printf("\nOpening Driver\n");
    fd = open("/dev/dht_device", O_RDWR);
    if(fd < 0) {
            printf("Cannot open device file...\n");
            return 0;
    }
 
    read(fd, dht_read_buf, 256);
    printf("%s\n",dht_read_buf);
    printf("Closing Driver\n");
    close(fd);
}