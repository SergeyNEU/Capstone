/*
Here's a basic example of how you can use the Raspberry Pi Camera Module V2 in C++ on a Raspberry Pi 4 Model 4 to capture an image:
*/
#include <raspicam/raspicam_cv.h>
using namespace std;
 
int main(int argc, char **argv) {
    raspicam::RaspiCam_Cv camera;
    camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!camera.open()) {
        cerr << "Error opening camera" << endl;
        return -1;
    }
    cv::Mat image;
    camera.grab();
    camera.retrieve(image);
    camera.release();
    cv::imwrite("image.jpg", image);
    return 0;
}
/*
NOTES:
This code uses the raspicam_cv library, which provides a C++ API for the Raspberry Pi Camera Module. The code opens the camera, sets the image format and resolution, captures an image, and saves it to a file named "image.jpg" in the current working directory.



----------------------------------------------------------------------------------------------------

If we use the machine learning I describe in machineLearningBrainstorm... the classifier is loaded from the file "classifier.xml" at the beginning of the program. Then, the camera is opened and continuously captures images. For each image, it extracts features, and then uses the classifier to predict whether the image contains a pothole or not.
*/

#include <raspicam/raspicam_cv.h>
#include <opencv2/ml.hpp>
#include <thread>
#include <atomic>
using namespace std;
using namespace cv;
using namespace cv::ml;

// Function to process images concurrently
void process_image(Mat &image, Ptr<SVM> &classifier, atomic<bool> &stop_flag) {
    while (!stop_flag) {
        Mat feature;
        // Extract features from image here
        // e.g. color histogram, texture, etc.
        float prediction = classifier->predict(feature);
        if (prediction == 1) {
            cout << "Pothole detected." << endl;
        } else {
            cout << "No pothole detected." << endl;
        }
    }
}

int main(int argc, char **argv) {
    // Load classifier from a file
    Ptr<SVM> classifier = SVM::load("classifier.xml");
    raspicam::RaspiCam_Cv camera;
    camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!camera.open()) {
        cerr << "Error opening camera" << endl;
        return -1;
    }
    // Use a flag to stop
the processing thread
atomic<bool> stop_flag(false);
// Create a thread for processing images concurrently
thread t(process_image, std::ref(image), std::ref(classifier), std::ref(stop_flag));
while (true) {
cv::Mat image;
camera.grab();
camera.retrieve(image);
}
// Stop the processing thread
stop_flag = true;
t.join();
camera.release();
return 0;
}

/*
NOTES:
In this example, I added a new function called "process_image" that takes an image, the classifier, and a stop flag as input. This function runs in a separate thread and continuously processes images as they come in, using the classifier to predict whether the image contains a pothole or not. The while loop in the main function captures images from the camera, and the processing thread uses this image to predict if it contains a pothole or not.

To stop the processing thread, I used an atomic boolean flag "stop_flag", this allows the main thread to set the flag to true, and the processing thread will stop running.

I also used the "thread" library to create a new thread for the function "process_image", this allows the processing of images to happen concurrently with the capturing of images.

Additionally, I used the "std::ref" function to pass the image, classifier, and stop_flag by reference, this means that the values passed to the thread will be updated in the main thread and will be visible to the processing thread.
*/