#include"CCamera.h"
#include <iostream> 


CCamera::CCamera()
{

}
CCamera::~CCamera()
{

}
bool CCamera::GetImage()
{
    int status = system("ffmpeg -f video4linux2 -s 1280x720 -input_format mjpeg -i /dev/video0 -vframes 1 image.jpg");

    if (status!=0)
        return false;
    else
        return true;
}