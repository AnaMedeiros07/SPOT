#ifndef __CSENSOR_H_
#define __CSENSOR_H_

class CSensor
{
    private:
        
        float value;
        float upper_limit;
        float lower_limit;
        int device_driver_id;
        
    public:
        
        CSensor();

        ~CSensor();

        void Set_Value(float);

        void Change_Limits(float up_limit,float l_limit);

        float Check_Sensor(void);

        int Sensor_Limits(void);
};


#endif
