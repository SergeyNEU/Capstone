#include <iostream>
#include <RTIMULib.h>

int main()
{
    // Instantiate a new RTIMU object
    RTIMUSettings settings("RTIMULib");
    RTIMU *imu = RTIMU::createIMU(&settings);

    // Initialize the IMU
    if (imu->IMUInit() < 0)
    {
        std::cerr << "Failed to initialize the IMU." << std::endl;
        return 1;
    }

    // Read and process the data in a loop
    while (true)
    {
        if (imu->IMURead() < 0)
        {
            std::cerr << "Failed to read data from the IMU." << std::endl;
            continue;
        }

        // Get the accelerometer and gyroscope data
        RTVector3 accel = imu->getAccel();
        RTVector3 gyro = imu->getGyro();

        // Process the data as desired
        std::cout << "Accelerometer: (" << accel.x() << ", " << accel.y() << ", " << accel.z() << ")" << std::endl;
        std::cout << "Gyroscope: (" << gyro.x() << ", " << gyro.y() << ", " << gyro.z() << ")" << std::endl;
    }

    // Clean up
    delete imu;
    return 0;
}

/*
#include <iostream>
#include <thread>
#include <chrono>
#include <sense_hat.h>
#include <vector>
#include <cmath>

using namespace std;
using namespace sense_hat;

vector<pair<double, array<double, 3>>> accelerometer_buffer;
vector<pair<double, array<double, 3>>> gyroscope_buffer;

// Function to read accelerometer data
void read_accelerometer() {
    SenseHat sense_hat;
    while (true) {
        // Read accelerometer data
        auto acceleration = sense_hat.get_accelerometer_raw();
        double x = acceleration.x;
        double y = acceleration.y;
        double z = acceleration.z;
        // Append data to buffer
        accelerometer_buffer.emplace_back(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count(), array<double,3>{x, y, z});
    }
}

// Function to read gyroscope data
void read_gyroscope() {
    SenseHat sense_hat;
    while (true) {
        // Read gyroscope data
        auto gyroscope = sense_hat.get_gyroscope_raw();
        double x = gyroscope.x;
        double y = gyroscope.y;
        double z = gyroscope.z;
        // Append data to buffer
        gyroscope_buffer.emplace_back(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count(), array<double,3>{x, y, z});
    }
}

int main() {
    // Create threads for reading accelerometer and gyroscope data
    thread accelerometer_thread(read_accelerometer);
    thread gyroscope_thread(read_gyroscope);

    // Start the threads
    accelerometer_thread.detach();
    gyroscope_thread.detach();

    // Main loop for image processing
    while (true) {
        // Capture image
        Mat image = capture_image();
        // Extract features from image
        Mat features = extract_features(image);
        // Use classifier to predict if image contains a pothole
        int prediction = classifier.predict(features);
        // Use statistical analysis to determine if car went over a pothole
        if (accelerometer_buffer.size() > 100) {
            // Get current time in milliseconds
            auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
            // Get last 100 samples of accelerometer data that were collected in the last 200 milliseconds
            vector<pair<double, array<double, 3>>> last_100_samples;
            for (auto i = accelerometer_buffer.rbegin(); i != accelerometer_buffer.rend() && last_100_samples.size() < 100; i++) {
                if (current_time - i->first < 200) {
                    last_100_samples.emplace_back(*i);
                }
            }
            // Calculate mean and standard deviation of last 100 samples
            array<double, 3> mean{0,0,0};
            array<double, 3> std{0,0,0};
            for (auto &i: last_100_samples) {
                mean[0] += i.second[0];
                mean[1] += i.second[1];
                mean[2] += i.second[2];
                std[0] += i.second[0] * i.second[0];
                std[1] += i.second[1] * i.second[1];
                std[2] += i.second[2] * i.second[2];
            }
            mean[0] /= last_100_samples.size();
            mean[1] /= last_100_samples.size();
            mean[2] /= last_100_samples.size();
            std[0] = sqrt(std[0] / last_100_samples.size() - mean[0] * mean[0]);
            std[1] = sqrt(std[1] / last_100_samples.size() - mean[1] * mean[1]);
            std[2] = sqrt(std[2] / last_100_samples.size() - mean[2] * mean[2]);
            // Define threshold for detecting a pothole
            array<double, 3> threshold{0.1, 0.1, 0.1};
            // Check if mean minus threshold is less than standard deviation for all axis
            if (mean[0] - threshold[0] < std[0] && mean[1] - threshold[1] < std[1] && mean[2] - threshold[2] < std[2]) {
                // Pothole detected
                cout << "Pothole detected." << endl;
            } else {
                cout << "No pothole detected." << endl;
            }
        }
        // Sleep for a short period to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    return 0;
}
*/

/*
NOTES:
The code first creates two threads, one for reading accelerometer data and another for reading gyroscope data. The threads are detached, which means they run independently of the main program and the program can exit without waiting for the threads to finish. The accelerometer and gyroscope data is stored in two separate buffers, one for accelerometer data and another for gyroscope data.

The main program enters a loop where it captures an image using the Raspberry Pi Camera Module V2-8 Megapixel, extracts features from the image, and uses a pre-trained classifier to predict if the image contains a pothole.

After that, the program uses statistical analysis to determine if the car went over a pothole. For this, it takes the last 100 samples of accelerometer data that were collected in the last 0.2 seconds, calculates the mean and standard deviation of the samples, defines a threshold for detecting a pothole, and checks if the mean minus the threshold is less than the standard deviation for all axis. If this is true, it means that the car went over a pothole, otherwise, it means that the car didn't go over a pothole.

Finally, the program sleeps for a short period to reduce CPU usage and repeat the process.
*/