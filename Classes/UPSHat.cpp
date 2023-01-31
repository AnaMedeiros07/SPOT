#include "UPSHat.h"
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <stddef.h>
//#include <linux/delay.h>


UPSHat::UPSHat()
{

    file = open("/dev/i2c-1", O_RDWR);

    if(file < 0)
    {
        exit(1);
    }    

    printf("Opened Device Driver\n");
}

UPSHat::~UPSHat()
{
    close(file);
}

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;
 
    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file,I2C_SMBUS,&args);
}

bool UPSHat::i2c_write(int addr, int bank_addr, int value)
{
    union i2c_smbus_data data;
    data.word = value;
    if( i2c_smbus_access(file,0,addr,bank_addr, &data))
        return false;
    else 
        return true;
}

float UPSHat::i2c_read(int addr, int bank_addr)
{
    union i2c_smbus_data data;
    int i;

    if (ioctl(file, I2C_SLAVE, UPS_ADDR) < 0) {
    /* ERROR HANDLING; */
    printf("Not Sucessful");
    exit(1);
    }
    
    int length = 4;
    char conversion[3];

    char buf[length];
    
    if (i2c_smbus_access(file,1,bank_addr,3,&data))
        return -1.0;
    else
        data.word = (data.word & 0x00FF)<<8 | (data.word & 0xFF00)>>8;
        sprintf(conversion,"%lu", data.word);
        printf("%s\n", conversion);
        return atof(conversion);

}


float UPSHat::get_voltage(void)
{
    float raw_adc = i2c_read(0x42,READ_ADDR);
    if ( raw_adc== -1.0){
        printf("Error reading\n");
        return -1000.0;
    }
    else
    {
		raw_adc = (float)((int)raw_adc>>3)*(float)(0.004);
        return raw_adc;
    }
}

float UPSHat::get_percentage(void)
{
    float voltage = get_voltage();
    float p = (voltage - 6.7)/1.4*100;
    if(p > 100)
        p = 100.0;
    if(p < 0)
        p = 0.0;

    return p;
}