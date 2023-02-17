#include "ICM20948.h"

// Function to initialize the motion sensor and check its type
int initializeMotionSensor();

// Function to get the sensor data
void getSensorData();

// Main Function
int main(int argc, char *argv[])
{
    int motionSensorType = initializeMotionSensor();
    if (motionSensorType == IMU_EN_SENSOR_TYPE_ICM20948)
    {
        printf("Motion sensor is ICM-20948\n");
        getSensorData();
    }
    else
    {
        printf("Motion sensor NULL\n");
    }

    return 0;
}

// Function to initialize the motion sensor and check its type
int initializeMotionSensor()
{
    IMU_EN_SENSOR_TYPE enMotionSensorType;
    IMU_ST_ANGLES_DATA stAngles;
    IMU_ST_SENSOR_DATA stGyroRawData;
    IMU_ST_SENSOR_DATA stAccelRawData;
    IMU_ST_SENSOR_DATA stMagnRawData;
    imuInit(&enMotionSensorType);
    return enMotionSensorType;
}

// Function to get the sensor data
void getSensorData()
{
    IMU_ST_ANGLES_DATA stAngles;
    IMU_ST_SENSOR_DATA stGyroRawData;
    IMU_ST_SENSOR_DATA stAccelRawData;
    IMU_ST_SENSOR_DATA stMagnRawData;
    while (1)
    {
        imuDataGet(&stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
        printf("\r\n /-------------------------------------------------------------/ \r\n");
        printf("\r\n Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n", stAngles.fRoll, stAngles.fPitch, stAngles.fYaw);
        printf("\r\n Acceleration: X: %d     Y: %d     Z: %d \r\n", stAccelRawData.s16X, stAccelRawData.s16Y, stAccelRawData.s16Z);
        printf("\r\n Gyroscope: X: %d     Y: %d     Z: %d \r\n", stGyroRawData.s16X, stGyroRawData.s16Y, stGyroRawData.s16Z);
        printf("\r\n Magnetic: X: %d     Y: %d     Z: %d \r\n", stMagnRawData.s16X, stMagnRawData.s16Y, stMagnRawData.s16Z);
        delay(1);
    }
}