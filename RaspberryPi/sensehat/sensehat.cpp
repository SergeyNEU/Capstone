#include "sensehat.hpp"

SenseHat::SenseHat() {}

int SenseHat::initializeMotionSensor()
{
    imuInit(&motionSensorType);
    return motionSensorType;
}

void SenseHat::getSensorData()
{
    imuDataGet(&angles, &gyroRawData, &accelRawData, &magnRawData);
    printf("\r\n /-------------------------------------------------------------/ \r\n");
    printf("\r\n Roll: %.2f Pitch: %.2f Yaw: %.2f \r\n", angles.roll, angles.pitch, angles.yaw);
    printf("\r\n Acceleration: X: %d Y: %d Z: %d \r\n", accelRawData.x, accelRawData.y, accelRawData.z);
    printf("\r\n Gyroscope: X: %d Y: %d Z: %d \r\n", gyroRawData.x, gyroRawData.y, gyroRawData.z);
    printf("\r\n Magnetic: X: %d Y: %d Z: %d \r\n", magnRawData.x, magnRawData.y, magnRawData.z);
    delay(1);
}

void SenseHat::imuInit(SensorType *motionSensorType)
{
    *motionSensorType = SENSOR_TYPE_ICM20948;
    // code to initialize the motion sensor
}

void SenseHat::imuDataGet(AnglesData *angles, SensorData *gyroRawData, SensorData *accelRawData, SensorData *magnRawData)
{
    // code to get sensor data
}

void SenseHat::delay(int ms)
{
    // code to delay
}