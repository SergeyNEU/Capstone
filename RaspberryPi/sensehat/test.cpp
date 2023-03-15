#include "sensehat.hpp"
#include "unistd.h"
#include "time.h"
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <iomanip>

int main(int argc, char *argv[])
{
    SenseHat sh;
    int motionSensorType;
    float sensorData[12];

    if (IMU_EN_SENSOR_TYPE_ICM20948 == sh.initializeMotionSensor())
    {
        printf("Motion sensor is ICM-20948\n");

        // Let sensors calibrate for 10 samples...
        for (int x = 0; x < 10; x++)
        {
            sh.getSensorData(sensorData);
            usleep(5000);
        }

        // Create unique CSV file name
        time_t rawtime;
        time(&rawtime);
        struct tm *timeinfo = localtime(&rawtime);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "sensor_data_%Y%m%d_%H%M%S.csv", timeinfo);
        std::string fileName(buffer);

        std::ofstream outFile(fileName);
        outFile << "Time, Roll, Pitch, Yaw, AX, AY, AZ, GX, GY, GZ, MX, MY, MZ\n";

        for (int x = 0; x < 10000; x++)
        {
            sh.getSensorData(sensorData);

            struct timeval tv;
            gettimeofday(&tv, NULL);
            struct tm *timeinfo = gmtime(&tv.tv_sec);

            printf("%02d:%02d.%03d | ", timeinfo->tm_min, timeinfo->tm_sec, (int)(tv.tv_usec / 1000));
            printf("Roll: %8.1f Pitch: %8.1f Yaw: %8.1f | ", sensorData[0], sensorData[1], sensorData[2]);
            // Acceleration
            printf("AX: %8.1f AY: %8.1f AZ: %8.1f | ", sensorData[3], sensorData[4], sensorData[5]);
            // Gyroscope
            printf("GX: %8.1f GY: %8.1f GZ: %8.1f | ", sensorData[6], sensorData[7], sensorData[8]);
            // Magnetic
            printf("MX: %8.1f MY: %8.1f MZ: %8.1f\n", sensorData[9], sensorData[10], sensorData[11]);

            outFile << timeinfo->tm_min << ":" << timeinfo->tm_sec << "." << (int)(tv.tv_usec / 1000) << ", ";
            outFile << sensorData[0] << ", " << sensorData[1] << ", " << sensorData[2] << ", ";
            outFile << sensorData[3] << ", " << sensorData[4] << ", " << sensorData[5] << ", ";
            outFile << sensorData[6] << ", " << sensorData[7] << ", " << sensorData[8] << ", ";
            outFile << sensorData[9] << ", " << sensorData[10] << ", " << sensorData[11] << "\n";

            usleep(5000);
        }

        outFile.close();
    }
    else
    {
        printf("Motion sensor not responding!\n");
    }

    return 0;
}