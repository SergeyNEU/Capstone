#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <string>
#include <iostream>
#include <string.h>
#include <unistd.h>

class Bluetooth {
public:
    Bluetooth();
    ~Bluetooth();
    int scan();
    std::string get_device_name(int index);
    bool connect_device(int index, int channel);
    bool send_message(const std::string& message);
    void disconnect();
private:
    int hciDeviceNumber_;
    int hciSocket_;
    int maxScanResults_ = 255;
    inquiry_info* scanResults_ = nullptr;
    int numScanResults_ = 0;
    uint8_t lap_[3] = {0x33, 0x8B, 0x9E}; // General/Unlimited Inquiry Access Code
    uint8_t scanLength_ = 8;              // Each unit is equal to 1.28 seconds
    int scanFlags_ = 0;                   // Inquiry flags
    int selectedDeviceIndex_ = -1;
    int rfcommSocket_ = -1;
};

#endif // BLUETOOTH_H