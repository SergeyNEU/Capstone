#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

// Function to capture image from camera and save it to file
int captureCameraImage(int width, int height, string fileName);

// Function to validate the width and height
void validateDimensions(int width, int height);

// Main Function
int main(int argc, char **argv)
{
    // Get the desired width and height from the user
    int width = 2240;
    int height = 2080;
    validateDimensions(width, height);

    // Capture image from camera and save it to file
    captureCameraImage(width, height, "camera.jpg");

    return 0;
}

// Function to capture image from camera and save it to file
int captureCameraImage(int width, int height, string fileName)
{
    // Open the camera device
    cv::VideoCapture cap(0, cv::CAP_V4L);
    if (!cap.isOpened())
    {
        cerr << "Error opening camera" << endl;
        return -1;
    }

    // Set the desired format, width, and height
    cap.set(cv::CAP_PROP_FORMAT, CV_8UC3);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    // Create a reusable Mat object for storing the captured image
    cv::Mat image;

    // Create a Rect object to define the Region of Interest
    cv::Rect roi(0, 0, width, height);

    // Capture a frame from the camera and save the ROI to a file
    if (cap.read(image))
    {
        cv::Mat roi_image = image(roi);
        cv::imwrite(fileName, roi_image);
    }
    else
    {
        cerr << "Error capturing frame from camera" << endl;
        return -1;
    }

    // Release the camera
    cap.release();
    return 0;
}

// Function to validate the width and height
void validateDimensions(int width, int height)
{
    if (width % 32 != 0 || height % 32 != 0)
    {
        cerr << "Error: width and height must be divisible by 32" << endl;
        exit(EXIT_FAILURE);
    }
    if (width > 3280 || height > 2464)
    {
        cerr << "Error: width and height cannot exceed 3280 and 2464, respectively" << endl;
        exit(EXIT_FAILURE);
    }
}