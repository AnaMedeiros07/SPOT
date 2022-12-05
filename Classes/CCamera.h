#ifndef __CCAMERA_H_
#define __CCAMERA_H_

#include <string.h>
#include <iostream>

using std::string;

class CCamera
{
    private:

    //imagem

    public:

       CCamera(void);

       ~CCamera(void);

       bool GetImage(void);

       bool OpenCamera(void);

       bool CloseCamera(void);

};


#endif