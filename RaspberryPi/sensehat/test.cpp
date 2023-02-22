#include "sensehat.hpp"

int main(int argc, char *argv[])
{
    SenseHat sh;
    int motionSensorType = sh.initializeMotionSensor();
    if (motionSensorType == IMU_EN_SENSOR_TYPE_ICM20948)
    {
        printf("Motion sensor is ICM-20948\n");
        float sensorData[12];
        while(true)
        {
            sh.getSensorData(sensorData);
            printf("\nSensor data:\n");
            printf("Roll: %.2f Pitch: %.2f Yaw: %.2f\n", sensorData[0], sensorData[1], sensorData[2]);
            printf("Acceleration: X: %.2f Y: %.2f Z: %.2f\n", sensorData[3], sensorData[4], sensorData[5]);
            printf("Gyroscope: X: %.2f Y: %.2f Z: %.2f\n", sensorData[6], sensorData[7], sensorData[8]);
            printf("Magnetic: X: %.2f Y: %.2f Z: %.2f\n", sensorData[9], sensorData[10], sensorData[11]);
            delay(1);
        }
    }
    else
    {
        printf("Motion sensor NULL\n");
    }

    return 0;
}
