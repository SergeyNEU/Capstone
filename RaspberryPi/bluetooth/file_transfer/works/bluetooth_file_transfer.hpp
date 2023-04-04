// This header file was created to integrate bluetooth_file_transfer_ftp_new.cpp into main.cpp
#ifndef BLUETOOTH_FILE_TRANSFER_HPP
#define BLUETOOTH_FILE_TRANSFER_HPP

#include <string>

bool transfer_file_over_bluetooth_ftp(const std::string &bt_addr, int ftp_channel, const std::string &file_path);

#endif // BLUETOOTH_FILE_TRANSFER_HPP
