#include "sensehat.hpp"

SenseHat::SenseHat() {}

int SenseHat::initializeMotionSensor()
{
    int counter = 0;
    float sensorData[12];
    do
    {
        imuInit(&motionSensorType);
        if (motionSensorType != IMU_EN_SENSOR_TYPE_ICM20948)
        {
            //printf("Motion sensor is not responding, retrying...\n");
            getSensorData(sensorData);
            usleep(5000);
        }
        counter++;
    } while ((motionSensorType != IMU_EN_SENSOR_TYPE_ICM20948) && (counter < 10));

    return motionSensorType;
}

void SenseHat::getSensorData(float *sensorData)
{
    imuDataGet(&angles, &gyroRawData, &accelRawData, &magnRawData);
    sensorData[0] = angles.fRoll;
    sensorData[1] = angles.fPitch;
    sensorData[2] = angles.fYaw;

    sensorData[3] = accelRawData.s16X;
    sensorData[4] = accelRawData.s16Y;
    sensorData[5] = accelRawData.s16Z;

    sensorData[6] = gyroRawData.s16X;
    sensorData[7] = gyroRawData.s16Y;
    sensorData[8] = gyroRawData.s16Z;
    sensorData[9] = magnRawData.s16X;
    sensorData[10] = magnRawData.s16Y;
    sensorData[11] = magnRawData.s16Z;
}

float SenseHat::movingAverage(std::deque<float> &values, float currentValue, int windowSize)
{
    if (values.size() >= windowSize)
    {
        values.pop_front();
    }

    values.push_back(currentValue);

    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

bool SenseHat::detectPothole(float originalValue, float currentValue, float *avgValue_beforePothole, float movementThreshold, float returnThreshold, MovementState &state, int &counter, int timeLimitSamples, int &cooldownCounter, int cooldownSamples)
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