#include <iostream>
#include "camera.hpp"

int main()
{
    int width = 2240; //2240 3264
    int height = 2080; //2080 2464
    Camera capture(width, height, "camera.jpg");
    capture.captureImage();
    return 0;
}