#include "CHouse.h"
#include "CSensor.h"
#include<iostream>
#include<vector>

using namespace std;
CHouse::CHouse(string name,string password)
        :name(name),password(password) {}


double CHouse::Sensor_Status(CSensor sensor)
{
    return sensor.Check_Sensor();
}

bool CHouse::Check_Login(string name , string password)
{
    if( name == this->name && password == this->password )
        return true;
    return false;
}
std::vector<double> CHouse::Check_Limits(CSensor temperature,CSensor humidity, CSensor smoke)
{
   std::vector<double> limits ={0.0,0.0,0.0,0.0,0.0}; 
   if( temperature.Sensor_Limits() ==1 )
   {
        limits[0] = temperature.Check_Sensor();
   }
   else if( temperature.Sensor_Limits() == 2 )
   {
        limits[3] = temperature.Check_Sensor();
   }
   if( smoke.Sensor_Limits()==1 )
   {
        limits[1] = smoke.Check_Sensor();
   }
   else if ( smoke.Sensor_Limits()==2 )
   {
        limits[4] = smoke.Check_Sensor();
   }
   if( humidity.Sensor_Limits()==1 )
   {
        limits[1] = smoke.Check_Sensor();
   }
   else if ( humidity.Sensor_Limits()==2 )
   {
        limits[4] = humidity.Check_Sensor();
   }

   return limits;
}