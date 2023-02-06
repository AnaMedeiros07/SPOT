#include <stdio.h>
#include "CSPOT.h"

using namespace std;

int main()
{
    CSPOT spot;

    spot.InitSemaphores();
    spot.InitSignal();
    spot.ConfigureServer();
    spot.ConfigureDatabase();
    spot.CreateThreads();

    while(1)
    {
        //execute program
    }
    return 0;
}