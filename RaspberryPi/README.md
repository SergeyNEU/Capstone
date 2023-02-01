# README - Sergey

## camera.cpp
C++ code for capturing images using the Raspberry Pi Camera Module V2 and using machine learning to classify the image as containing a pothole or not.

## machineLearning.cpp
C++ code for loading, preparing, and training a machine learning model on a dataset of images of potholes and non-potholes, and using the trained model to make predictions on new images.

## sensehat.cpp
C++ code for using a Raspberry Pi Sense HAT to read accelerometer and gyroscope data, and using the data in combination with image processing techniques to detect and classify potholes.

## bluetooth2ndStage.cpp
C++ code for initializing and configuring a Bluetooth connection, as well as code for sending and receiving data over the connection. This file is likely to contain more robust methods for handling the bluetooth communication and additional features that can be added to the project.

## gps.cpp
C++ code for setting up the USB adapter, installing a GPS Daemon (gpsd) and disabling the gpsd systemd service for Raspbian Jessie or later releases. After running gpsd manually, the GPS device can be tested to get data from the GPS module.