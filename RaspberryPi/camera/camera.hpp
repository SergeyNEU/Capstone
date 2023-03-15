#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>

class Camera
{
public:
    Camera(int width, int height, std::string filename);
    int captureImage();

private:
    int m_width;
    int m_height;
    std::string m_filename;
};

#endif