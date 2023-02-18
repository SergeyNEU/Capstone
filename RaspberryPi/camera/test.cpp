#include <iostream>
#include "camera.hpp"

int main()
{
    int width = 2240;
    int height = 2080;
    CameraCapture capture(width, height, "camera.jpg");
    capture.captureImage();
    return 0;
}