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
        
        CSensor();

        ~CSensor();

        void Set_Value(double);

        void Change_Limits(double,double);

        double Check_Sensor(void);

        int Sensor_Limits(void);
};


#endif
