#include "sensehat.hpp"

SenseHat::SenseHat() {}

int SenseHat::initializeMotionSensor()
{
    imuInit(&motionSensorType);
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