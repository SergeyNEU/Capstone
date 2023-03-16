#ifndef SENSEHAT_H
#define SENSEHAT_H

#include <stdio.h>
#include "ICM20948.h"
#include "unistd.h"
#include "time.h"
#include <sys/time.h>
#include <deque>
#include <numeric>

typedef enum
{
    SENSOR_TYPE_NULL,
    SENSOR_TYPE_ICM20948
} SensorType;

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} AnglesData;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} SensorData;

enum class MovementState
{
    Idle,
    MovedDown,
    Cooldown
};

class SenseHat
{
public:
    SenseHat();
    int initializeMotionSensor();
    void getSensorData(float *sensorData);
    float movingAverage(std::deque<float> &values, float currentValue, int windowSize);
    bool detectPothole(float originalValue, float currentValue, float *avgValue_beforePothole, float movementThreshold, float returnThreshold, MovementState &state, int &counter, int timeLimitSamples, int &cooldownCounter, int cooldownSamples);

private:
    IMU_EN_SENSOR_TYPE motionSensorType;
    IMU_ST_ANGLES_DATA angles;
    IMU_ST_SENSOR_DATA gyroRawData;
    IMU_ST_SENSOR_DATA accelRawData;
    IMU_ST_SENSOR_DATA magnRawData;
};

#endif