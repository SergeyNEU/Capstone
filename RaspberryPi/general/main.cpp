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

// // GPS functions
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

// Convert img to string
std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string imageToString(const std::string& imagePath) {
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Error: Could not read the image file." << std::endl;
        return "";
    }

    std::vector<uchar> buf;
    cv::imencode(".jpg", img, buf);
    return base64_encode
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

        for (int x = 0; x < 10000; x++)
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
                    printGPSData(parser.processGGAValues(ggaValues));
                }

                // Capture image creates two background processes that captures image and processes it.
                std::string filename = "./images/image_" + std::to_string(x) + ".jpg";
                captureImage(filename);
                
                imageToString(filename);
                
            }
        }
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }
    return 0;
}
