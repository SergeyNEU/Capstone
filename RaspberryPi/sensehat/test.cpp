// #include "sensehat.hpp"
// #include "unistd.h"
// #include "time.h"
// #include <sys/time.h>

// int main(int argc, char *argv[])
// {
//     SenseHat sh;
//     int motionSensorType;

//     do
//     {
//         motionSensorType = sh.initializeMotionSensor();
//         if (motionSensorType != IMU_EN_SENSOR_TYPE_ICM20948){
//             printf("Motion sensor NULL...\n");
//         }
//         sleep(1);
//     } while (motionSensorType != IMU_EN_SENSOR_TYPE_ICM20948);

//     if (motionSensorType == IMU_EN_SENSOR_TYPE_ICM20948)
//     {
//         printf("Motion sensor is ICM-20948\n");
//         float sensorData[12];
//         for (int x = 0; x < 10000; x++)
//         {
//             sh.getSensorData(sensorData);

//             struct timeval tv;
//             gettimeofday(&tv, NULL);
//             struct tm *timeinfo = gmtime(&tv.tv_sec);

//             printf("%02d:%02d.%03d | ", timeinfo->tm_min, timeinfo->tm_sec, (int)(tv.tv_usec / 1000));
//             printf("Roll: %8.1f Pitch: %8.1f Yaw: %8.1f | ", sensorData[0], sensorData[1], sensorData[2]);
//             // Acceleration
//             printf("AX: %8.1f AY: %8.1f AZ: %8.1f | ", sensorData[3], sensorData[4], sensorData[5]);
//             // Gyroscope
//             printf("GX: %8.1f GY: %8.1f GZ: %8.1f | ", sensorData[6], sensorData[7], sensorData[8]);
//             // Magnetic
//             printf("MX: %8.1f MY: %8.1f MZ: %8.1f\n", sensorData[9], sensorData[10], sensorData[11]);

//             //usleep(10000);
//         }
//     }
//     else
//     {
//         printf("Motion sensor NULL\n");
//     }

//     return 0;
// }

#include <cstdio>
#include <ctime>
#include <vector>
#include "sensehat.hpp"
#include "unistd.h"
#include "time.h"
#include <sys/time.h>

struct SensorDataStorage
{
    std::time_t timestamp;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
};

int main(int argc, char *argv[])
{
    // Initialize SenseHat
    SenseHat sh;
    int motionSensorType = sh.initializeMotionSensor();
    if (motionSensorType != IMU_EN_SENSOR_TYPE_ICM20948)
    {
        std::printf("Motion sensor NULL\n");
        return 1;
    }

    // Store sensor data in vector
    std::vector<SensorDataStorage> sensorDataVec;
    SensorDataStorage data;
    struct timeval tv;
    for (int x = 0; x < 100; x++)
    {
        // Get sensor data
        float sensorData[12];
        sh.getSensorData(sensorData);

        // Store sensor data and timestamp
        data.timestamp = std::time(nullptr);
        data.ax = sensorData[3];
        data.ay = sensorData[4];
        data.az = sensorData[5];
        data.gx = sensorData[6];
        data.gy = sensorData[7];
        data.gz = sensorData[8];

        sensorDataVec.push_back(data);
        usleep(100000);
    }

    // Plot sensor data using gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == nullptr)
    {
        std::printf("Failed to open gnuplot pipe\n");
        return 1;
    }

    std::fprintf(gnuplotPipe, "set ylabel 'Angular Velocity/Acceleration'\n");
    std::fprintf(gnuplotPipe, "plot '-' using 1:2 title 'Gx' with lines, "
                              "'-' using 1:2 title 'Gy' with lines, "
                              "'-' using 1:2 title 'Gz' with lines, "
                              "'-' using 1:2 title 'Ax' with lines, "
                              "'-' using 1:2 title 'Ay' with lines, "
                              "'-' using 1:2 title 'Az' with lines\n");

    for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.gx);
    }
    std::fprintf(gnuplotPipe, "e\n");

    for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.gy);
    }
    std::fprintf(gnuplotPipe, "e\n");

    for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.gz);
    }
    std::fprintf(gnuplotPipe, "e\n");

        for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.ax);
    }
    std::fprintf(gnuplotPipe, "e\n");

    for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.ay);
    }
    std::fprintf(gnuplotPipe, "e\n");

    for (const auto &data : sensorDataVec)
    {
        std::fprintf(gnuplotPipe, "%ld %f\n", data.timestamp, data.az);
    }
    std::fprintf(gnuplotPipe, "e\n");

    std::fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}