#include "power_hat.h"
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c.h>

CPower_hat::CPower_hat(/* args */)
{

    file = open("/dev/i2c-1", O_RDWR);

    if(file < 0)
    {
        exit(1);
    }    

    printf("Opened Device Driver\n");
}

CPower_hat::~CPower_hat()
{

}

int CPower_hat::i2c_read(int addr, int bank_addr)
{
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
    /* ERROR HANDLING; */
    printf("Not Sucessful");
    exit(1);
    }
    
    int length = 2;

    printf("Inserted Addr %d %d \n",addr, bank_addr);

    char buf[length];
    buf[0]= bank_addr;

    if(read(file,buf,length) != length)
    {
    // ERROR HANDLING - I2C transaction failed   
    printf("Read Failed\n");
    }

    for(int i=0;i<length;i++)
        printf("%x    space\n", buf[i]);

    return buf[0];

    //return i2c_smbus_read_word_data(file, bank_addr);
}

int CPower_hat::i2c_write(int addr, int data)
{
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
    /* ERROR HANDLING; */
    exit(1);
    }

    return 0;
}

int CPower_hat::get_shunt_voltage(void)
{
    return i2c_read(0x42,0x00);
}

int CPower_hat::get_bus_voltage(void)
{
    return i2c_read(POWER_HAT_ADDR,0x02);
}

int CPower_hat::get_power(void)
{
    return i2c_read(POWER_HAT_ADDR,POWER);
}

int CPower_hat::get_current(void)
{
    return i2c_read(POWER_HAT_ADDR,CURRENT);
}