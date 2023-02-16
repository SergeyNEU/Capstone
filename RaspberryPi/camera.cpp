#include <iostream>
#include <opencv2/opencv.hpp>

// Compilation Commands
// g++ camera.cpp -O3 -o camera `pkg-config --cflags --libs opencv4`

using namespace std;

int main(int argc, char **argv) {
    // Get the desired width and height from the user
    int width = 2240;
    int height = 2080;

    if (width % 32 != 0 || height % 32 != 0) {
        cerr << "Error: width and height must be divisible by 32" << endl;
        return -1;
    }
    if (width > 3280 || height > 2464) {
        cerr << "Error: width and height cannot exceed 3280 and 2464, respectively" << endl;
        return -1;
    }

    // Open the camera device
    cv::VideoCapture cap(0, cv::CAP_V4L);
    if (!cap.isOpened()) {
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
    if (cap.read(image)) {
        cv::Mat roi_image = image(roi);
        cv::imwrite("Output/camera.jpg", roi_image);
    } else {
        cerr << "Error capturing frame from camera" << endl;
        return -1;
    }

    // Release the camera
    cap.release();
}