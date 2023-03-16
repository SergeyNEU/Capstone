#include "sensehat.hpp"
#include "unistd.h"
#include <iostream>
#include <deque>
#include <numeric>

int main(int argc, char *argv[])
{
    SenseHat sh;
    float sensorData[12];

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