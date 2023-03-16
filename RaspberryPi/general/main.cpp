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

// Camera functions
void captureImage(const std::string &filename)
{
    int width = 4608;
    int height = 2592;

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
        int windowSize = 10;
        float movementThreshold = 150.0;
        float returnThreshold = 15.0;
        int timeLimitSamples = 40;
        int cooldownSamples = 15;

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

                // Capture image
                std::string filename = "./image_" + std::to_string(x) + ".jpg";
                captureImage(filename);

                // Get GPS coordinates
                std::vector<std::string> ggaValues = getGPSData(parser);
                if (!ggaValues.empty())
                {
                    printGPSData(ggaValues);
                }
                usleep(1000);
            }
        }
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }
    return 0;
}