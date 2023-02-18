#include "sensehat.hpp"

int main(int argc, char *argv[])
{
    SenesHat sh;
    int motionSensorType = sh.initializeMotionSensor();
    if (motionSensorType == IMU_EN_SENSOR_TYPE_ICM20948)
    {
        printf("Motion sensor is ICM-20948\n");
        sh.getSensorData();
    }
    else
    {
        printf("Motion sensor NULL\n");
    }

    return 0;
}