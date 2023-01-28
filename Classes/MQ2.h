#ifndef __MQ2_H_
#define __MQ2_H_

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class MQ2
{
    private:

        int mq2_file;

    public:

        MQ2(void);
        
        ~MQ2(void);

        int getStatus(void);
};

#endif