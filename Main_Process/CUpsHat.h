#ifndef __CUPS_HAT_H__
#define __CUPS_HAT_H__

#include "smbus.h"
#include <unistd.h>				
#include <fcntl.h>				
#include <sys/ioctl.h>			
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>

//UPS Hat i2c addresses
#define POWER_HAT_ADDR      0x42

#define CONFIGURATION_ADDR  0x00
#define SHUNT_VOLTAGE       0x01
#define BUS_VOLTAGE         0x02
#define POWER               0x03
#define CURRENT             0x04
#define CALIBRATION         0x05

//To calibrate the voltage - percentage calculation
#define MAX_VOLTAGE 8.2
#define MIN_VOLTAGE 6.4
#define VOLTAGE_RANGE (MAX_VOLTAGE-MIN_VOLTAGE)

class CUpsHat
{
    private:

        int ups_file;
        float voltage = 0;
        float percentage = 0;

        __s32 read_i2c_addr(__u8 addr);

        __s32 write_i2c_addr(__u8 addr, __u16 word);

        __u16 swap_bytes(__u16 word);

    public:

        CUpsHat();

        ~CUpsHat();

        int updateValues();

        float getVoltage();

        float getPercentage();

};

#endif