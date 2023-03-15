#include "../camera/camera.hpp"
#include "../gps/gps.hpp"
#include "../sensehat/sensehat.hpp"

int main()
{
    int width = 2240;
    int height = 2080;
    Camera capture(width, height, "camera.jpg");
    capture.captureImage();

    GPS parser;
    parser.parse();

    // Close serial port
    close(parser.serialPort);

    SenseHat sh;
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