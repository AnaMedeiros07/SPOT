#ifndef ___UPS_HAT_H__
#define __UPS_HAT_H__

#define UPS_ADDR      0x42

#define CONFIGURATION_ADDR  0x00
#define READ_ADDR       0x02

 
#include <linux/types.h>
#include <sys/ioctl.h>
#include <stddef.h>


class UPSHat
{
    private:
        
        int file;

        float i2c_read(int, int);
        
        

    public:
        UPSHat();
        
        ~UPSHat();
        
        bool i2c_write(int,int,int);

        float get_voltage(void);
        
        float get_percentage(void);
        
        int get_smoke(void);
};

#endif