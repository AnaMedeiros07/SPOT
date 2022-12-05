#ifndef __CMOTION_H_
#define __CMOTION_H_

#include <ctime>

using std::time_t;

class CMotion
{
    private:

    time_t last_reset;
    double time_since_reset;

    public:

       CMotion(void);

       ~CMotion(void);

       bool TurnOnCamera(void);

       time_t GetTime(void);

       void ResetTimeCount(void);

};


#endif