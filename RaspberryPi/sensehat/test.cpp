#include "sensehat.hpp"
#include "unistd.h"
#include <iostream>
#include <deque>
#include <numeric>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

int main(int argc, char *argv[])
{
    SenseHat sh;
    float sensorData[12];

    // Create a CSV file named with the current timestamp
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::stringstream timestamp_ss;
    timestamp_ss << std::put_time(std::localtime(&start_time), "%Y-%m-%d-%H-%M-%S");
    std::string filename = "sensor_data_" + timestamp_ss.str() + ".csv";

    std::ofstream csvFile(filename);
    if (!csvFile.is_open())
    {
        std::cerr << "Failed to create CSV file: " << filename << std::endl;
        return 1;
    }

    csvFile << "Timestamp,SensorData,AvgValue,PotholeDetected" << std::endl;

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
        float movementThreshold = 100.0;
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

            auto now = std::chrono::system_clock::now();
            std::time_t current_time = std::chrono::system_clock::to_time_t(now);
            std::stringstream current_timestamp_ss;
            current_timestamp_ss << std::put_time(std::localtime(&current_time), "%Y-%m-%d-%H-%M-%S");

            if (state == MovementState::Idle)
            {
                avgValue = sh.movingAverage(values, currentValue, windowSize);
            }

            bool potholeDetected = false;
            if (values.size() >= windowSize && sh.detectPothole(avgValue, currentValue, &avgValue_beforePothole, movementThreshold, returnThreshold, state, counter, timeLimitSamples, cooldownCounter, cooldownSamples))
            {
                printf("Raspberry Pi has detected a pothole at %s.\n", current_timestamp_ss.str().c_str());
                potholeDetected = true;
                usleep(1000);
            }

            // Save sensor data and pothole detection status to CSV file
            csvFile << current_timestamp_ss.str() << "," << currentValue << "," << avgValue  << "," << (potholeDetected ? "1" : "0") << std::endl;
        }
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }

    // Close the CSV file
    csvFile.close();

    return 0;
}