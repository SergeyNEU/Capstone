#include "camera.hpp"

Camera::Camera(int width, int height, const std::string& fileName)
    : width_(width), height_(height), fileName_(fileName), cap_(0, cv::CAP_V4L)
{
    validateDimensions(width, height);
    if (!cap_.isOpened())
    {
        std::cerr << "Error opening camera" << std::endl;
        exit(EXIT_FAILURE);
    }
    cap_.set(cv::CAP_PROP_FORMAT, CV_8UC3);
    cap_.set(cv::CAP_PROP_FRAME_WIDTH, width_);
    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, height_);
}

int Camera::captureImage()
{
    cv::Mat image;
    cv::Rect roi(0, 0, width_, height_);
    if (cap_.read(image))
    {
        cv::Mat roi_image = image(roi);
        cv::imwrite(fileName_, roi_image);
    }
    else
    {
        std::cerr << "Error capturing frame from camera" << std::endl;
        return -1;
    }
    cap_.release();
    return 0;
}

void Camera::validateDimensions(int width, int height)
{
    if (width % 32 != 0 || height % 32 != 0)
    {
        std::cerr << "Error: width and height must be divisible by 32" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (width > 3280 || height > 2464)
    {
        std::cerr << "Error: width and height cannot exceed 3280 and 2464, respectively" << std::endl;
        exit(EXIT_FAILURE);
    }
}