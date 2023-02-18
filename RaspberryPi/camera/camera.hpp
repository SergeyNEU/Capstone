#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <opencv2/opencv.hpp>

class Camera {
public:
    Camera(int width, int height, const std::string& fileName);
    int captureImage();

private:
    void validateDimensions(int width, int height);

    int width_;
    int height_;
    std::string fileName_;
    cv::VideoCapture cap_;
};

#endif // CAMERA_H