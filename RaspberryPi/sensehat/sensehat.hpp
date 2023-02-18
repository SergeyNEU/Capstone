#ifndef SENESHAT_H
#define SENESHAT_H

#include <stdio.h>
#include "ICM20948.h"

typedef enum
{
    IMU_EN_SENSOR_TYPE_NULL,
    IMU_EN_SENSOR_TYPE_ICM20948
} IMU_EN_SENSOR_TYPE;

typedef struct
{
    float fRoll;
    float fPitch;
    float fYaw;
} IMU_ST_ANGLES_DATA;

typedef struct
{
    int16_t s16X;
    int16_t s16Y;
    int16_t s16Z;
} IMU_ST_SENSOR_DATA;

class SenesHat
{
public:
    SenesHat();
    int initializeMotionSensor();
    void getSensorData();

private:
    IMU_EN_SENSOR_TYPE enMotionSensorType;
    IMU_ST_ANGLES_DATA stAngles;
    IMU_ST_SENSOR_DATA stGyroRawData;
    IMU_ST_SENSOR_DATA stAccelRawData;
    IMU_ST_SENSOR_DATA stMagnRawData;
    void imuInit(IMU_EN_SENSOR_TYPE *penMotionSensorType);
    void imuDataGet(IMU_ST_ANGLES_DATA *pstAngles, IMU_ST_SENSOR_DATA *pstGyroRawData, IMU_ST_SENSOR_DATA *pstAccelRawData, IMU_ST_SENSOR_DATA *pstMagnRawData);
    void delay(int ms);
};

#endif