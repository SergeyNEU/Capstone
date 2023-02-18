#ifndef SENESHAT_H
#define SENESHAT_H

#include <stdio.h>
#include "ICM20948.h"

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

class SenseHat
{
public:
    SenseHat();
    int initializeMotionSensor();
    void getSensorData();

private:
    SensorType motionSensorType;
    AnglesData angles;
    SensorData gyroRawData;
    SensorData accelRawData;
    SensorData magnRawData;
    void imuInit(SensorType *motionSensorType);
    void imuDataGet(AnglesData *angles, SensorData *gyroRawData, SensorData *accelRawData, SensorData *magnRawData);
    void delay(int ms);
};

#endif