#!/bin/bash
# Set the script interpreter to Bash

# Script Name: connect_android.sh
# Description: This script pairs, trusts, and connects a Linux system to an Android phone
#              using Bluetooth.

# Set the Android phone's Bluetooth address (case sensitive)
PHONE_BT_ADDRESS="B4:F1:DA:66:C3:A5"

# Load the btusb kernel module to enable Bluetooth USB devices support
sudo modprobe btusb

# Start the Bluetooth system service
sudo systemctl start bluetooth

# Check if the phone is already a trusted device by looking for its Bluetooth address in the list of devices
is_trusted=$(bluetoothctl devices | grep "$PHONE_BT_ADDRESS")

# If the phone is not a trusted device, start the pairing process
if [ -z "$is_trusted" ]; then
  # Display a message to indicate the pairing process has started
  echo "Pairing with your phone..."

  # Enable Bluetooth on the system
  bluetoothctl power on

  # Enable the Bluetooth pairing agent
  bluetoothctl agent on

  # Set the default Bluetooth pairing agent
  bluetoothctl default-agent

  # Start scanning for nearby Bluetooth devices
  bluetoothctl scan on

  # Continuously check if the phone has been discovered by looking for its Bluetooth address in the list of devices
  while [ -z "$(bluetoothctl devices | grep "$PHONE_BT_ADDRESS")" ]; do
    # Wait for 1 second before checking again
    sleep 1
  done

  # Stop scanning for nearby Bluetooth devices
  bluetoothctl scan off

  # Pair with the phone using its Bluetooth address
  bluetoothctl pair "$PHONE_BT_ADDRESS"

  # Trust the phone using its Bluetooth address
  bluetoothctl trust "$PHONE_BT_ADDRESS"

  # Display a message to indicate the phone has been paired and trusted
  echo "Phone paired and trusted."
fi

# Check if the phone is connected by looking for "Connected: yes" in the phone's info
is_connected=$(bluetoothctl info "$PHONE_BT_ADDRESS" | grep "Connected: yes")

# If the phone is not connected, initiate the connection process
if [ -z "$is_connected" ]; then
  # Display a message to indicate the connection process has started
  echo "Connecting to your phone..."

  # Connect to the phone using its Bluetooth address
  bluetoothctl connect "$PHONE_BT_ADDRESS"

  # Continuously check if the phone has been connected by looking for "Connected: yes" in the phone's info
  while [ -z "$(bluetoothctl info "$PHONE_BT_ADDRESS" | grep "Connected: yes")" ]; do
    # Wait for 1 second before checking again
    sleep 1
  done

  # Display a message to indicate the phone has been connected successfully
  echo "Connected successfully!"
else
  # Display a message to indicate the phone is already connected
  echo "Already connected!"
fi
