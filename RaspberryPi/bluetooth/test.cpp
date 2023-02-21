#include "bluetooth.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    try
    {
        Bluetooth bt;

        // Scan for devices
        std::cout << "Scanning for devices..." << std::endl;
        int numDevices = bt.scan();
        std::cout << "Found " << numDevices << " devices." << std::endl;

        // List the devices
        for (int i = 0; i < numDevices; i++)
        {
            std::cout << "[" << i << "] " << bt.get_device_name(i) << std::endl;
        }

        // Connect to a device
        std::cout << "Enter the index of the device to connect to: ";
        int index;
        std::cin >> index;
        std::cout << "Connecting to device " << index << "..." << std::endl;
        if (!bt.connect_device(index, 1))
        {
            std::cerr << "Error connecting to device" << std::endl;
            return 1;
        }

        // Send a message to the device
        std::string message;
        std::cout << "Enter a message to send: ";
        std::cin.ignore();
        std::getline(std::cin, message);
        if (!bt.send_message(message))
        {
            std::cerr << "Error sending message to device" << std::endl;
            return 1;
        }

        // Disconnect from the device
        std::cout << "Disconnecting from device..." << std::endl;
        bt.disconnect();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}