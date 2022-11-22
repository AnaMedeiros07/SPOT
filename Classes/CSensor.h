#ifndef __CSENSOR_H_
#define __CSENSOR_H_

class CSensor
{
    private:
        
        double value;
        double upper_limit;
        double lower_limit;
        int device_driver_id;
        
    public:
        
        CSensor(double, double, int);

        ~CSensor();

        void Set_Value(double);

        int Change_Limits(double);

        double Check_Sensor(void);

        int Sensor_Limits(void);
};


#endif