#include "sensehat.hpp"
#include "unistd.h"
#include <iostream>
#include <deque>
#include <numeric>

enum class MovementState
{
    Idle,
    MovedDown,
    Cooldown
};

float movingAverage(std::deque<float> &values, float currentValue, int windowSize)
{
    if (values.size() >= windowSize)
    {
        values.pop_front();
    }
    values.push_back(currentValue);

    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

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
        int windowSize = 7;              // Adjust this value to change the moving average window size
        float movementThreshold = 300.0; // Adjust this value for sensitivity
        float returnThreshold = 200.0;   // Adjust this value for sensitivity
        int timeLimitSamples = 15;       // Adjust this value to change the time limit for returning to the original position
        int cooldownSamples = 20;        // Adjust this value to change the cooldown period

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
                // Calculate the moving average of values
                avgValue = movingAverage(values, currentValue, windowSize);
            }

            if (values.size() >= windowSize && detectPothole(avgValue, currentValue, &avgValue_beforePothole, movementThreshold, returnThreshold, state, counter, timeLimitSamples, cooldownCounter, cooldownSamples))
            {
                printf("Raspberry Pi has detected a pothole.\n");

                usleep(5000);
            }
        }
        else
        {
            printf("Motion sensor not responding!\n");
        }
        return 0;
    }
