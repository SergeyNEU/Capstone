#include "sensehat.hpp"
#include "unistd.h"
#include "time.h"
#include <sys/time.h>

int main(int argc, char *argv[])
{
    SenseHat sh;
    int motionSensorType = sh.initializeMotionSensor();
    if (motionSensorType == IMU_EN_SENSOR_TYPE_ICM20948)
    {
        printf("Motion sensor is ICM-20948\n");
        float sensorData[12];
        for(int x = 0; x < 100; x++)
        {
            sh.getSensorData(sensorData);

            struct timeval tv;
            gettimeofday(&tv, NULL);
            struct tm *timeinfo = gmtime(&tv.tv_sec);

            printf("%02d:%02d.%03d | ", timeinfo->tm_min, timeinfo->tm_sec, (int) (tv.tv_usec/1000));
            printf("Roll: %8.1f Pitch: %8.1f Yaw: %8.1f | ",  sensorData[0], sensorData[1], sensorData[2]);
            // Acceleration
            printf("AX: %8.1f AY: %8.1f AZ: %8.1f | ", sensorData[3], sensorData[4], sensorData[5]);
            // Gyroscope
            printf("GX: %8.1f GY: %8.1f GZ: %8.1f | ", sensorData[6], sensorData[7], sensorData[8]);
            // Magnetic
            printf("MX: %8.1f MY: %8.1f MZ: %8.1f\n", sensorData[9], sensorData[10], sensorData[11]);
        }
    }
    else
    {
        printf("Motion sensor NULL\n");
    }

    return 0;
}
