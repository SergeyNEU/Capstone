#include "../camera/camera.hpp"
#include "../gps/gps.hpp"
#include "../sensehat/sensehat.hpp"
#include "unistd.h"
#include <iostream>
#include <deque>
#include <numeric>
#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <bitset>
#include <ctime>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iostream>
#include <chrono>
#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <openssl/evp.h>

std::string base64_encode(const std::string &input)
{
    // Calculate the size of the output buffer, which is 4/3 the size of the input buffer.
    size_t input_length = input.length();
    size_t output_length = 4 * ((input_length + 2) / 3);

    // Allocate the output buffer.
    std::string output(output_length, 0);

    // Perform the base64 encoding.
    unsigned char *input_ptr = reinterpret_cast<unsigned char *>(const_cast<char *>(input.data()));
    unsigned char *output_ptr = reinterpret_cast<unsigned char *>(const_cast<char *>(output.data()));
    EVP_EncodeBlock(output_ptr, input_ptr, input_length);

    return output;
}

// Camera functions
int captureImage(const std::string &filename)
{
    int width = 1280; // 1920
    int height = 720; // 1080

    Camera camera(width, height, filename);
    return camera.captureImage();
}

// GPS functions
void configureGPS(GPS &parser)
{
    bool serialPortConfiguration = parser.configureSerialPort();

    if (serialPortConfiguration)
    {
        printf("GPS: Configured (/dev/ttyUSB0)\n");
    }
    else
    {
        printf("GPS: Not Configured (/dev/ttyUSB0)\n");
    }
}

std::vector<std::string> getGPSData(GPS &parser)
{
    std::vector<std::string> ggaValues = parser.parse();

    if (ggaValues.empty())
    {
        printf("GPS: No output\n");
    }

    return ggaValues;
}

void printGPSData(const std::vector<std::string> &ggaValues)
{
    GPS parser;
    parser.printGGAValues(ggaValues);
}

void sendToBluetooth(const std::vector<long long> &millisBuffer, const std::vector<std::vector<std::string>> &gpsBuffer)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "sendToBluetooth Error: Failed to fork the process" << std::endl;
    }
    else if (pid == 0)
    {
        // Child process
        std::string imagesDirectory = "./images/";

        for (size_t i = 0; i < millisBuffer.size(); ++i)
        {
            std::string timestamp = std::to_string(millisBuffer[i]);
            std::string txtFilename = imagesDirectory + "image_" + timestamp + ".txt";
            std::string csvFilename = imagesDirectory + "pothole_image_" + timestamp + ".csv";
            std::string processedImageFilename = imagesDirectory + "image_" + timestamp + "_processed.jpg";

            // Continuously check if the corresponding .txt file exists and is not empty
            bool fileFound = false;
            int timeout_val = 0;
            int timeout_limit = 150;
            while (!fileFound && timeout_val < timeout_limit)
            {
                std::ifstream txtFile(txtFilename);
                if (txtFile)
                {
                    usleep(500000); // 500 ms
                    std::string line;
                    std::getline(txtFile, line);
                    if (line == "")
                    {
                        timeout_val = timeout_limit + 1;
                        std::cout << "SendToBluetooth: No pothole processed" << std::endl;
                        continue;
                    }

                    fileFound = true;
                    std::cerr << "sendToBluetooth: Pothole found" << std::endl;

                    // Extract the second value (certainty percentage) from the file
                    std::istringstream iss(line);
                    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
                    float certaintyValue = std::stof(tokens[1]);

                    // Convert the processed image to base64
                    std::ifstream imageFile(processedImageFilename, std::ios::binary);
                    std::string imageString((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());
                    std::string base64Image = base64_encode(imageString);

                    // Write buffered data to CSV file
                    const auto &processedData = gpsBuffer[i];
                    std::ofstream csvFile(csvFilename);
                    csvFile << processedData[1] << "," << processedData[2] << "," << processedData[3] << "," << processedData[5] << "," << processedData[6] << "," << processedData[7] << "," << certaintyValue << "," << base64Image;
                    csvFile.close();

                    const std::string bt_addr = "B4:F1:DA:66:C3:A5"; // Bluetooth address for the Android phone (case sensitive)
                    int ftp_channel = 12;                            // FTP channel number. Make sure it is the correct channel number.
                    std::cout << "SendToBluetooth: About to Send via BT" << std::endl;

                    // Declare a stringstream variable to store the obexftp command string
                    std::stringstream command;
                    // Populate the command string with the appropriate parameters
                    command << "obexftp --nopath --noconn --uuid none --bluetooth " << bt_addr << " --channel " << ftp_channel << " -p " << csvFilename;
                    // Execute the command using the system() function and store the result in an int variable
                    int result = std::system(command.str().c_str());
                }
                else
                {
                    // Sleep for a while before checking again
                    usleep(100000); // 100 ms
                    ++timeout_val;  // Timeout after 100ms*200 = 20sec
                }
            }
            if (timeout_val == timeout_limit)
            {
                std::cout << "SendToBluetooth: Process timed out!" << std::endl;
            }
        }
                            
        timestampBuffer.clear();
        gpsBuffer.clear();

        exit(0); // Ensure this child process exits after completing its tasks
    }
    else
    {
        // Parent process
        std::cout << "SendToBluetooth process started. Process ID: " << pid << std::endl;
    }
}

// Function: isBluetoothConnected
// Purpose:  Checks if there is a valid Bluetooth connection with a specific device
// Returns:  bool - true if the connection exists, false otherwise
bool isBluetoothConnected()
{
    // Define the Bluetooth address for the target device, which is the Android phone (case sensitive)
    const std::string bt_addr = "B4:F1:DA:66:C3:A5"; // I believe this variable is previously defined. If so, delete this line. Redundant.

    // Create a stringstream variable to store the hcitool command string
    std::stringstream command;

    // Populate the command string with the appropriate parameters to
    // find established connections with the target Bluetooth address
    command << "hcitool con | grep " << bt_addr;

    // Execute the command using the system() function and store the result in an int variable
    // The system() function returns 0 if the command was successful (i.e., the connection was found)
    int result = std::system(command.str().c_str());

    // Return true if the result is 0, meaning that the connection exists; otherwise, return false
    return (result == 0);
}

long long getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

int main(int argc, char *argv[])
{
    SenseHat sh;
    float sensorData[12];

    // Initialize GPS
    GPS parser;
    configureGPS(parser);

    std::vector<std::string> processedGPSData;

    if (IMU_EN_SENSOR_TYPE_ICM20948 == sh.initializeMotionSensor())
    {
        printf("Motion Sensor: Configured (ICM-20948)\n");

        for (int x = 0; x < 10; x++)
        {
            sh.getSensorData(sensorData);
            usleep(5000);
        }

        std::deque<float> values;
        int windowSize = 20;
        float movementThreshold = 100.0;
        float returnThreshold = 15.0;
        int timeLimitSamples = 40;
        int cooldownSamples = 125;

        MovementState state = MovementState::Idle;
        int counter = 0;
        int cooldownCounter = 0;
        float avgValue_beforePothole = 0;
        float avgValue = 0;
        std::vector<long long> timestampBuffer;
        std::vector<std::vector<std::string>> gpsBuffer;

        printf("Waiting for potholes...\n");
        for (int x = 0; x < 100000; x++)
        {
            sh.getSensorData(sensorData);
            float currentValue = sensorData[4];

            if (state == MovementState::Idle)
            {
                avgValue = sh.movingAverage(values, currentValue, windowSize);
            }

            if (values.size() >= windowSize && sh.detectPothole(avgValue, currentValue, &avgValue_beforePothole, movementThreshold, returnThreshold, state, counter, timeLimitSamples, cooldownCounter, cooldownSamples))
            {

                printf("Raspberry Pi has detected a pothole!\n");

                std::vector<std::string> ggaValues = getGPSData(parser);
                if (!ggaValues.empty())
                {
                    processedGPSData = parser.processGGAValues(ggaValues);
                }

                // Capture image creates two background processes that captures image and processes it.
                auto time = getTimestamp();
                std::string filename = "./images/image_" + std::to_string(time) + ".jpg";
                captureImage(filename);
                timestampBuffer.push_back(time);
                gpsBuffer.push_back(processedGPSData);

                if (isBluetoothConnected())
                {
                    // Buffers also cleared after sending
                    sendToBluetooth(timestampBuffer, gpsBuffer);
                }
                else
                {
                    std::cout << "SendToBluetooth: No connection" << std::endl;
                }
            }
        }
    }
    else
    {
        printf("Motion Sensor: Not configured\n");
    }
    return 0;
}