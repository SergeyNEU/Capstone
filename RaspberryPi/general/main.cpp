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
#include <opencv2/opencv.hpp>
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
#include <base64.h>

const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// Camera functions
void captureImage(const std::string &filename)
{
    int width = 1920;
    int height = 1080;

    Camera camera(width, height, filename);
    camera.captureImage();
}

// GPS functions
void configureGPS(GPS &parser)
{
    bool serialPortConfiguration = parser.configureSerialPort();

    if (serialPortConfiguration)
    {
        printf("Configured serial port (/dev/ttyUSB0).\n");
    }
    else
    {
        printf("Unable to configure serial port (/dev/ttyUSB0).\n");
    }
}

std::vector<std::string> getGPSData(GPS &parser)
{
    std::vector<std::string> ggaValues = parser.parse();

    if (ggaValues.empty())
    {
        printf("No serial port output.\n");
    }

    return ggaValues;
}

void printGPSData(const std::vector<std::string> &ggaValues)
{
    GPS parser;
    parser.printGGAValues(ggaValues);
}

void sendToBluetooth(const std::string &timestamp, const std::vector<std::string> &processedData)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "Error: Failed to fork the process for sendToBluetooth." << std::endl;
    }
    else if (pid == 0)
    {
        // Child process

        std::string imagesDirectory = "./images/";
        std::string txtFilename = imagesDirectory + "image_" + timestamp + ".txt";
        std::string csvFilename = imagesDirectory + "image_" + timestamp + ".csv";
        std::string processedImageFilename = imagesDirectory + "image_" + timestamp + "_processed.jpg";

        // Continuously check if the corresponding .txt file exists and is not empty
        bool fileFound = false;
        while (!fileFound)
        {
            std::ifstream txtFile(txtFilename);
            if (txtFile && !txtFile.peek() == std::ifstream::traits_type::eof())
            {
                fileFound = true;

                std::string line;
                std::getline(txtFile, line);

                // Extract the second value (certainty percentage) from the file
                std::istringstream iss(line);
                std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
                int certaintyValue = std::stoi(tokens[1]);

                // Convert the processed image to base64
                std::ifstream imageFile(processedImageFilename, std::ios::binary);
                std::string imageString((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());
                std::string base64Image = base64::encode(imageString);

                // Create a CSV file with the certainty value and the base64 converted image-to-string value
                std::ofstream csvFile(csvFilename);
                csvFile << processedData[1] << "," << processedData[2] << "," << processedData[3] << "," << processedData[5] << "," << processedData[6] << "," << processedData[7] << "," << certaintyValue << "," << base64Image;
                csvFile.close();
            }
            else
            {
                // Sleep for a while before checking again
                usleep(100000); // 100 ms
            }
        }

        exit(0); // Ensure this child process exits after completing its tasks
    }
    else
    {
        // Parent process
        std::cout << "SendToBluetooth process started. Process ID: " << pid << std::endl;
    }
}

int main(int argc, char *argv[])
{
    SenseHat sh;
    float sensorData[12];

    // Initialize GPS
    GPS parser;
    configureGPS(parser);

    if (IMU_EN_SENSOR_TYPE_ICM20948 == sh.initializeMotionSensor())
    {
        printf("Motion sensor is ICM-20948\n");

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
        std::vector<std::string> processedGPSData;

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

                printf("Raspberry Pi has detected a pothole.\n");

                // Get GPS coordinates
                std::vector<std::string> ggaValues = getGPSData(parser);
                if (!ggaValues.empty())
                {
                    processedGPSData = parser.processGGAValues(ggaValues)
                    printGPSData(processedGPSData);
                }

                // Capture image creates two background processes that captures image and processes it.
                auto now = std::chrono::system_clock::now();
                auto duration = now.time_since_epoch();
                auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
                std::string filename = "./images/image_" + std::to_string(millis) + ".jpg";
                captureImage(filename);

                //Create background that looks for image, .txt, and _processed image to send.
                sendToBluetooth(millis, processedGPSData);
            }
        }
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }
    return 0;
}
