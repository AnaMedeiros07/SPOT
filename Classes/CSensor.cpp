#include"CSensor.h"

CSensor::CSensor()
{
}
CSensor::~CSensor()
{

}

void CSensor::Set_Value(double value)
{
    this->value = value;
}

void CSensor::Change_Limits(double up_limit,double l_limit)
{
    this->upper_limit = up_limit;
    this->lower_limit = l_limit;
}

double CSensor::Check_Sensor()
{
 return this->value;
}

int CSensor::Sensor_Limits(void)
{
    int check = 0;
    if(this->value >= this->upper_limit)
    {
        check = 2;
    }
    else if (this->value <= this->lower_limit)
    {
        check = 1;
    }

    return check;
}