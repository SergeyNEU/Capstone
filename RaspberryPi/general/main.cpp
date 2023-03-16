#include "../camera/camera.hpp"
#include "../gps/gps.hpp"
#include "../sensehat/sensehat.hpp"
#include "unistd.h"
#include <iostream>
#include <deque>
#include <numeric>
#include <vector>
#include <string>

enum class MovementState
{
    Idle,
    MovedDown,
    Cooldown
};

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

// Moving average function
float movingAverage(std::deque<float> &values, float currentValue, int windowSize, MovementState state)
{
    if (state == MovementState::Idle)
    {
        if (values.size() >= windowSize)
        {
            values.pop_front();
        }
        values.push_back(currentValue);
    }

    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

// Pothole detection function
bool detectPothole(float originalValue, float currentValue, float *avgValue_beforePothole, float movementThreshold, float returnThreshold, MovementState &state, int &counter, int timeLimitSamples, int &cooldownCounter, int cooldownSamples)
{
    if (state == MovementState::Idle && (originalValue - currentValue) > movementThreshold)
    {
        state = MovementState::MovedDown;
        counter = 0;
        *avgValue_beforePothole = originalValue;
    }
    else if (state == MovementState::MovedDown)
    {
        counter++;
        if (*avgValue_beforePothole - currentValue <= returnThreshold)
        {
            state = MovementState::Cooldown;
            cooldownCounter = 0;
            return true;
        }
        else if (counter >= timeLimitSamples)
        {
            state = MovementState::Idle;
        }
    }
    else if (state == MovementState::Cooldown)
    {
        cooldownCounter++;
        if (cooldownCounter >= cooldownSamples)
        {
            state = MovementState::Idle;
        }
    }

    return false;
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

        // Calibrate for 10 samples
        for (int x = 0; x < 10; x++)
        {
            sh.getSensorData(sensorData);
            usleep(5000);
        }

        std::deque<float> values;
        int windowSize = 7; // Adjust this value to change the moving average window size
        float movementThreshold =
            300.0;                     // Adjust this value for sensitivity
        float returnThreshold = 200.0; // Adjust this value for sensitivity
        int timeLimitSamples = 15;     // Adjust this value to change the time limit for returning to the original position
        int cooldownSamples = 20;      // Adjust this value to change the cooldown period

        MovementState state = MovementState::Idle;
        int counter = 0;
        int cooldownCounter = 0;
        float avgValue_beforePothole = 0;

        for (int x = 0; x < 10000; x++)
        {
            sh.getSensorData(sensorData);
            float currentValue = sensorData[4];

            // Calculate the moving average of values
            float avgValue = movingAverage(values, currentValue, windowSize, state);

            if (values.size() >= windowSize && detectPothole(avgValue, currentValue, &avgValue_beforePothole, movementThreshold, returnThreshold, state, counter, timeLimitSamples, cooldownCounter, cooldownSamples))
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
            }

            usleep(5000);
        }
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }

    // Close serial port
    close(parser.serialPort);
    return 0;
}