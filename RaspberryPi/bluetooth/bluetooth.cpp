#include "bluetooth.hpp"

Bluetooth::Bluetooth()
{
    // Get the HCI device number
    hciDeviceNumber_ = hci_get_route(nullptr);
    if (hciDeviceNumber_ < 0)
    {
        std::cerr << "Error getting HCI device number" << std::endl;
        throw std::runtime_error("Error getting HCI device number");
    }

    // Open the HCI device
    hciSocket_ = hci_open_dev(hciDeviceNumber_);
    if (hciSocket_ < 0)
    {
        std::cerr << "Error opening HCI device" << std::endl;
        throw std::runtime_error("Error opening HCI device");
    }
}

Bluetooth::~Bluetooth()
{
    // Free the memory used by the scan results
    free(scanResults_);

    // Close the HCI device
    close(hciSocket_);
}

int Bluetooth::scan()
{
    // Start the scan
    numScanResults_ = hci_inquiry(hciDeviceNumber_, scanLength_, maxScanResults_, lap_, &scanResults_, scanFlags_);
    if (numScanResults_ < 0)
    {
        std::cerr << "Error starting scan" << std::endl;
        throw std::runtime_error("Error starting scan");
    }
    return numScanResults_;
}

std::string Bluetooth::get_device_name(int index)
{
    if (index < 0 || index >= numScanResults_)
    {
        std::cerr << "Invalid device index" << std::endl;
        throw std::runtime_error("Invalid device index");
    }
    char macAddress[18] = {0};
    ba2str(&scanResults_[index].bdaddr, macAddress);
    char deviceName[248] = {0};
    if (hci_read_remote_name(hciSocket_, &scanResults_[index].bdaddr, sizeof(deviceName), deviceName, 0) < 0)
    {
        strcpy(deviceName, "[unknown]");
    }
    std::string result = macAddress;
    result += " ";
    result += deviceName;
    return result;
}

bool Bluetooth::connect_device(int index, int channel)
{
    if (index < 0 || index >= numScanResults_)
    {
        std::cerr << "Invalid device index" << std::endl;
        return false;
    }
    // Connect to the selected device
    sockaddr_rc deviceAddress = {0};
    deviceAddress.rc_family = AF_BLUETOOTH;
    deviceAddress.rc_channel = (uint8_t)channel;
    bacpy(&deviceAddress.rc_bdaddr, &(scanResults_[index].bdaddr));
    rfcommSocket_ = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (rfcommSocket_ < 0)
    {
        std::cerr << "Error opening RFCOMM socket" << std::endl;
        return false;
    }
    if (connect(rfcommSocket_, (sockaddr *)&deviceAddress, sizeof(deviceAddress)) < 0)
    {
        std::cerr << "Error connecting to device" << std::endl;
        return false;
    }
    selectedDeviceIndex_ = index;
    return true;
}

bool Bluetooth::send_message(const std::string &message)
{
    if (selectedDeviceIndex_ < 0 || selectedDeviceIndex_ >= numScanResults_)
    {
        std::cerr << "No device is currently connected" << std::endl;
        return false;
    }
    if (write(rfcommSocket_, message.c_str(), message.length()) < 0)
    {
        std::cerr << "Error sending message to device" << std::endl;
        return false;
    }
    return true;
}

void Bluetooth::disconnect()
{
    // Close the connection
    close(rfcommSocket_);
    rfcommSocket_ = -1;
    selectedDeviceIndex_ = -1;
}
