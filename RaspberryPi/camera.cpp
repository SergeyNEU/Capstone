#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
 
int main(int argc, char **argv) {
    cv::VideoCapture cap(0, cv::CAP_V4L);
    if (!cap.isOpened()) {
        cerr << "Error opening camera" << endl;
        return -1;
    }
    cap.set(cv::CAP_PROP_FORMAT, CV_8UC3);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640+32*50);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480+32*50);
    cv::Mat image;
    cap.read(image);
    cap.release();
    cv::imwrite("image.jpg", image);
    return 0;
}

// Compilation Commands
/*g++ camera.cpp -O3 -o camera `pkg-config --cflags --libs opencv4`*/