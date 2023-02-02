#include <stdio.h>
#include "CSPOT.h"

using namespace std;

int main()
{
    CSPOT spot;

    spot.InitSemaphores();
    spot.InitSignal();
    spot.CreateThreads();
    
    while(1)
    {
        
    }
    return 0;
}