#include <stdio.h>
#include "CSPOT.h"

using namespace std;

int main()
{
    CSPOT spot;

    spot.InitSemaphores();
    spot.CreateThreads();
    spot.InitSignal();
    while(1)
    {
        
    }
    return 0;
}