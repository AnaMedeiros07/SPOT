#ifndef __CHB100_H_
#define __CHB100_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <semaphore.h>

#define SIGETX 44

#define REG_CURRENT_TASK _IOW('a','a',int32_t*)

class CHb100
{
    private:

        int fd;
        static sem_t motion_sem;
        
        void static sig_event_handler(int, siginfo_t *, void *);
    
    public:

        CHb100();

        ~CHb100();

        int AssociateSem(sem_t);

        
};

#endif