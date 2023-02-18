#include "sensehat.hpp"

SenesHat::SenesHat() {}

int SenesHat::initializeMotionSensor()
{
    imuInit(&enMotionSensorType);
    return enMotionSensorType;
}

void SenesHat::getSensorData()
{
    imuDataGet(&stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
    printf("\r\n /-------------------------------------------------------------/ \r\n");
    printf("\r\n Roll: %.2f Pitch: %.2f Yaw: %.2f \r\n", stAngles.fRoll, stAngles.fPitch, stAngles.fYaw);
    printf("\r\n Acceleration: X: %d Y: %d Z: %d \r\n", stAccelRawData.s16X, stAccelRawData.s16Y, stAccelRawData.s16Z);
    printf("\r\n Gyroscope: X: %d Y: %d Z: %d \r\n", stGyroRawData.s16X, stGyroRawData.s16Y, stGyroRawData.s16Z);
    printf("\r\n Magnetic: X: %d Y: %d Z: %d \r\n", stMagnRawData.s16X, stMagnRawData.s16Y, stMagnRawData.s16Z);
    delay(1);
}

void SenesHat::imuInit(IMU_EN_SENSOR_TYPE *penMotionSensorType)
{
    *penMotionSensorType = IMU_EN_SENSOR_TYPE_ICM20948;
    // code to initialize the motion sensor
}

void SenesHat::imuDataGet(IMU_ST_ANGLES_DATA *pstAngles, IMU_ST_SENSOR_DATA *pstGyroRawData, IMU_ST_SENSOR_DATA *pstAccelRawData, IMU_ST_SENSOR_DATA *pstMagnRawData)
{
    // code to get sensor data
}

void SenesHat::delay(int ms)
{
    // code to delay
}