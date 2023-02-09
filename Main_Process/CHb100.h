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
    
    public:

        CHb100();

        ~CHb100();

        void CreateSignalHandler(void(*)(int, siginfo_t*, void*));

};

#endif