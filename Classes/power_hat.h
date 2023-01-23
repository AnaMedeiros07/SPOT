#ifndef __POWER_HAT_H__
#define __POWER_HAT_H__

#define POWER_HAT_ADDR      0x42

#define CONFIGURATION_ADDR  0x00
#define SHUNT_VOLTAGE       0x01
#define BUS_VOLTAGE         0x02
#define POWER               0x03
#define CURRENT             0x04
#define CALIBRATION         0x05


class CPower_hat
{
private:
    
    int file;

    int i2c_read(int, int);
    
    int i2c_write(int, int);

public:
    CPower_hat();
    
    ~CPower_hat();
    
    int get_shunt_voltage(void);
    
    int get_bus_voltage(void);
    
    int get_power(void);
    
    int get_current(void);
};

#endif