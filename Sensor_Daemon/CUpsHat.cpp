#include "CUpsHat.h"

CUpsHat::CUpsHat()
{
	if ((ups_file = open("/dev/i2c-1", O_RDWR)) < 0)
	{
		exit(1);
	}
}

CUpsHat::~CUpsHat()
{
    close(ups_file);
}

__s32 CUpsHat::read_i2c_addr(__u8 addr)
{
    if (ioctl(ups_file, I2C_SLAVE, POWER_HAT_ADDR) < 0)
        {
            printf("Failed to acquire bus access and/or talk to slave.\n");
            //ERROR HANDLING; you can check errno to see what went wrong
            return -1;
        }

    __s32 res;

    res = i2c_smbus_read_word_data(ups_file, addr);
        if (res < 0) {
            /* ERROR HANDLING: i2c transaction failed */
        } else {
            /* res contains the read word */
        }
    
    return res;

}

__s32 CUpsHat::write_i2c_addr(__u8 addr, __u16 word)
{
    if (ioctl(ups_file, I2C_SLAVE, POWER_HAT_ADDR) < 0)
        {
            printf("Failed to acquire bus access and/or talk to slave.\n");
            //ERROR HANDLING; you can check errno to see what went wrong
            return -1;
        }

    __s32 res;

    res = i2c_smbus_write_word_data(ups_file, addr, word);
    if (res < 0) {
            /* ERROR HANDLING: i2c transaction failed */
            return -1;
    } 
    return 0;
}


__u16 CUpsHat::swapBytes(__u16 word)
{
    word = (word & 0x00FF)<<8 | (word & 0xFF00)>>8;
    return word;
}

int CUpsHat::updateValues(void)
{
    __u16 volt = read_i2c_addr(BUS_VOLTAGE);
    if(volt < 0)
        return 1;

    volt = swapBytes(volt);

    voltage = (float)((int)volt>>3)*(float)(0.004);

    percentage = (voltage - MIN_VOLTAGE)/VOLTAGE_RANGE*100;

    if(percentage < 0)
        percentage = 0;
    else if (percentage > 100)
        percentage = 100;

    return 0;
}

float CUpsHat::getVoltage()
{
    return voltage;
}

float CUpsHat::getPercentage()
{
    return percentage;
}

