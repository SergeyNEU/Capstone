# Raspberry Pi Pothole Detection System

## Overview
Welcome to the Pothole Protector - a Raspberry Pi Pothole Detection System. This project integrates hardware components, software applications, and machine learning to identify potholes using a Raspberry Pi setup. It includes a Raspberry Pi Camera for image capture, a Sense HAT for sensor data, machine learning for pothole detection, Bluetooth connectivity, and GPS functionalities. The project also features a user-friendly Android app and a supportive website for enhanced interaction and data visualization.

## Components
- **Raspberry Pi Camera**: Captures real-time road images.
- **Sense HAT**: Gathers accelerometer and gyroscope data.
- **Machine Learning**: Identifies potholes from camera feed using a trained model.
- **Bluetooth Connectivity**: Manages data transmission.
- **GPS Module**: Provides location tracking.
- **Android App**: Displays potholes on a map.
- **Supportive Website**: Complements the app with additional functionalities.

## Getting Started with Raspberry Pi

### Prerequisites
- Raspberry Pi 4 Model B board.
- PISugar 2 Pro Battery.
- Sense HAT (B) for Raspberry Pi.
- SD Card with Raspberry Pi OS installed.
- Peripherals: Keyboard, mouse, and monitor.

### Setup Guide
1. **Initial Setup**: Follow the [Raspberry Pi Setup Video](https://www.youtube.com/watch?v=LWfCAADZSUM&ab_channel=RasimMuratovic) to assemble and power up your Raspberry Pi.
2. **OS Installation**: Download and install the Raspberry Pi OS from [Raspberry Pi Software](https://www.raspberrypi.com/software/). Avoid LITE versions to ensure GUI availability.
3. **Battery Setup**: Refer to the [PiSugar 2 Pro Wiki](https://github.com/PiSugar/PiSugar/wiki/PiSugar2-Plus) for battery configuration.

### Boot-up Instructions
- Insert the SD card, connect the peripherals, and power up.
- Log in (default: `username: pi`, `password: raspberry`).
- Run `sudo raspi-config` for initial configuration.

## Software Installation
1. **Clone the Repository**: Clone this repository to your Raspberry Pi.
2. **Install Dependencies**: Run `sudo apt-get update` and `sudo apt-get upgrade`.
3. **Compile C++ Code**: Use the provided Makefile or a C++ compiler to compile the individual components.

## Android Studio App
- The app displays potholes on a map with a modern UI.
- Download and install the app on your Android device.
- Connect the app to the Raspberry Pi system for real-time data visualization.

## Website
- The website provides an extended view and analysis of the detected potholes.
- Access the website for detailed reports and historical data.

## Usage
- Choose the desired module (camera, Sense HAT, etc.) to start collecting data.
- The machine learning model processes the camera feed to detect potholes.
- Use the Android app and website for monitoring and analysis.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
