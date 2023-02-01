#include <iostream>
#include <thread>
#include <chrono>
#include <sense_hat.h>
#include <vector>
#include <cmath>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;
using namespace sense_hat;

// Constants
const int NUM_SAMPLES = 100;
const int NUM_MILLIS = 200;
const array<double, 3> THRESHOLD = {0.1, 0.1, 0.1};
const int SLEEP_PERIOD = 10;

// Buffers for storing sensor data
vector<pair<double, array<double, 3>>> accelerometer_buffer;
vector<pair<double, array<double, 3>>> gyroscope_buffer;
vector<tuple<time_t, double, double, double, Mat>> pothole_buffer;

//Get unique Raspberry Pi 4 Model B ID
std::string get_device_id() {
    // Open the "cpuinfo" file in the "/proc" directory
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    std::string device_id;

    // Iterate through each line of the file
    while (std::getline(file, line)) {
        // Check if the line contains the "Serial" field
        if (line.find("Serial") != std::string::npos) {
            // Extract device id
            device_id = line.substr(line.find(":") + 2);
            break;
        }
    }
    file.close();
    return device_id;
}

// Thread function to read accelerometer data
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

// Thread function to read gyroscope data
void read_gyroscope() {
    SenseHat sense_hat;
    while (true) {
        // Read gyroscope data
        auto gyroscope = sense_hat.get_gyroscope_raw();
        double x = gyroscope.x;
        double y = gyroscope.y;
        double z = gyroscope.z;
        // Append data to buffer
        gyroscope_buffer.emplace_back(chrono::duration_cast<chrono::milliseconds>(chrono::system
clock::now().time_since_epoch()).count(), array<double,3>{x, y, z});
    }
}

int main() {

    // Get device id
    std::string device_id = get_device_id();

    // Start threads to read accelerometer and gyroscope data
    thread accelerometer_thread(read_accelerometer);
    thread gyroscope_thread(read_gyroscope);
    accelerometer_thread.detach();
    gyroscope_thread.detach();

    // Bluetooth setup
    struct sockaddr_rc addr = { 0 };
    int sock, client;
    socklen_t size;

    // allocate socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_bdaddr = *BDADDR_ANY;
    addr.rc_channel = (uint8_t) 1;

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    // Listen for incoming connections
    listen(sock, 1);
    bool connected = false;
    // Main loop for image processing and Bluetooth communication
    while (true) {
        // Capture image
        Mat image = capture_image();
        // Extract features from image
        Mat features = extract_features(image);
        // Use classifier to predict if image contains a pothole and the probability of the prediction
        pair<int,double> prediction = classifier.predictProb(features);
        // Use statistical analysis to determine if car went over a pothole
        if (accelerometer_buffer.size() > NUM_SAMPLES) {
            // Get current time in milliseconds
            auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
            // Get last NUM_SAMPLES samples of accelerometer data that were collected in the last NUM_MILLIS milliseconds
            vector<pair<double, array<double, 3>>> last_samples;
            for (auto i = accelerometer_buffer.rbegin(); i != accelerometer_buffer.rend() && last_samples.size() < NUM_SAMPLES; i++) {
                if (current_time - i->first < NUM_MILLIS) {
                    last_samples.emplace_back(*i);
                }
            }
            // Calculate mean and standard deviation of last NUM_SAMPLES samples
            array<double, 3> mean{0,0,0};
            array<double, 3> std{0,0,0};
            for (auto &i: last_samples) {
                mean[0] += i.second[0];
                mean[1] += i.second[1];
                mean[2] += i.second[2];
                std[0] += i.second[0] * i.second[0];
                std[1] += i.second[1] * i.second[1];
                std[2] += i.second[2] * i.second[2];
            }
            mean[0] /= last_samples.size();
            mean[1] /= last_samples.size();
            mean[2] /= last_samples.size();
            std[0] = sqrt(std[0] / last_samples.size() - mean[0] * mean[0]);
            std[1] = sqrt(std[1] / last_samples.size() - mean[1] * mean[1]);
            std[2] = sqrt(std[2] / last_samples.size() - mean[2] * mean[2]);
            // Check if mean minus threshold is less than standard deviation for all axis
            if (mean[0] - THRESHOLD[0] < std[0] && mean[1] - THRESHOLD[1] < std[1] && mean[2] - THRESHOLD[2] < std[2]) {
                // Pothole detected
                cout << "Pothole detected." << endl;
                time_t timestamp = time(0);
                if(connected){
                    // Send message to connected device
                    char message[256];
                    sprintf(message, "Pothole detected at %s with probability %.2f", ctime(&timestamp), prediction.second);
                    send(client, message, sizeof(message), 0);
                    // Send image to connected device
                    send(client, image.data, image.total() * image.elemSize(), 0);
                }else{
                    // Store detection in buffer
                    pothole_buffer.emplace_back(timestamp, prediction.second, mean[0], mean[1], mean[2], image);
                }
                // Wait for incoming connection
                client = accept(sock, (struct sockaddr *)&addr, &size);
                connected = true;
            } else {
                cout << "No pothole detected." << endl;
                // Check if there's any pothole detection stored in the buffer
                if(!pothole_buffer.empty()){
                    // Send message to connected device
                    char message[256];
                    auto detection = pothole_buffer.front();
                    sprintf(message, "Pothole detected at %s with probability %.2f", ctime(&get<0>(detection)), get<1>(detection));
                    send(client, message, sizeof(message), 0);
                    // Send image to connected device
                    send(client, get<5>(detection).data, get<5>(detection).total() * get<5>(detection).elemSize(), 0);
                    // remove sent detection from buffer
                    pothole_buffer.erase(pothole_buffer.begin());
                }
            }
        }
        // Sleep for a short period to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(SLEEP_PERIOD));
    }
    return 0;
}


/*
NOTES:
The main function of the code is to process the image captured by the camera and use sensor data from the Sense HAT to detect potholes. To achieve this, the code uses several libraries and techniques such as:

Image processing libraries: These libraries are used to extract features from the image captured by the camera. The features are used to train a classifier that can recognize potholes in the image.

Classifier: The classifier is trained using a dataset of images of potholes and non-potholes. The classifier is then used to predict if an image contains a pothole and the probability of the prediction.

Sensor data: The Sense HAT is equipped with an accelerometer and a gyroscope. The code uses these sensors to collect data about the motion of the car. The sensor data is analyzed using statistical methods to detect potholes.

Statistical analysis: The code uses statistical methods to determine if a car has gone over a pothole. This is done by calculating the mean and standard deviation of the sensor data collected over a short period of time. If the mean minus a threshold is less than the standard deviation for all axis, the code considers it a pothole detection.

Bluetooth communication: The Raspberry Pi communicates with a connected device via Bluetooth, sending the pothole detection information, the image and timestamp of the detection. If there's no connection it stores the detection in a buffer and sends the detection information once a connection is established.

Multithreading: To increase the efficiency of the code, multithreading is used to read sensor data and process the image simultaneously.

Global constants: The global constants are defined to fine-tune the pothole detection algorithm. For example, the number of samples collected, the amount of milliseconds, the threshold for determining a pothole and the sleep period can be adjusted as needed.

Sleep function: The code also includes a sleep_for function that makes the code sleep for a short period of time (10 milliseconds) to reduce CPU usage.
*/