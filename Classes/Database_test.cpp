#include"CDatabase.h"

#define DATABASE "DATABASE.db"

using namespace std;
int main()
{
    CDatabase database;

    //database.createDB(DATABASE);
    database.createTable(DATABASE);


    database.insertDataSensorID("humidity","18.0","30.0","10.0");
    //database.insertDataSensorID("smoke","1","0","0");
    //database.insertDataSensorID("motion","0","0","0");
    //database.insertDataSensorID("battery","80","0","0");
    //database.insertDataUserID("Ana","1234","9999");

    //database.GetNumber("Ana");
    cout << database.ProcessRequest("Limits")<<endl;

    return 0;
}