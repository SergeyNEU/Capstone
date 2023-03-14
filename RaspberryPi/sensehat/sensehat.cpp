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

    /*  The unit of acceleration measured by the demo is LSB (least significant bit), 
        and the unit is often converted to gravitational acceleration (g) in actual use. 
        The sample demo of the module sets AFS_SEL=0 by default, and the corresponding range 
        is 16384 LSB/g (±2g), so the actual acceleration measured is: */
    
    sensorData[3] = accelRawData.s16X/16384;
    sensorData[4] = accelRawData.s16Y/16384;
    sensorData[5] = accelRawData.s16Z/16384;

    /*  The unit of angular velocity measured by the program is LSB (least significant bit).
        In practice, the unit is often converted to angular velocity (°/sec). The sample program 
        of the module sets FS_SEL=2 by default, and the corresponding range is 32.8 LSB/(°/s) (±1000°/s), 
        so the actual angular velocity measured is
    */

    sensorData[6] = gyroRawData.s16X/32.8;
    sensorData[7] = gyroRawData.s16Y/32.8;
    sensorData[8] = gyroRawData.s16Z/32.8;
    sensorData[9] = magnRawData.s16X;
    sensorData[10] = magnRawData.s16Y;
    sensorData[11] = magnRawData.s16Z;
}