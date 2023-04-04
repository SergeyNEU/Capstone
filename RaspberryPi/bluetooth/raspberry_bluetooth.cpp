#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv) {
    // Declare and initialize the sockaddr_rc struct for the remote device's address
    struct sockaddr_rc addr = { 0 };
    int status, s;
    char dest[18] = "XX:XX:XX:XX:XX:XX"; // Replace with your Android device's Bluetooth address

    // Allocate a socket using the Bluetooth address family, stream socket type, and RFCOMM protocol
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // Set the connection parameters, specifying the address family, Bluetooth address, and channel
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(dest, &addr.rc_bdaddr); // Convert the string address to a bdaddr_t struct

    // Connect to the remote Android device using the given address and socket
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // Check if the connection was successful
    if (status < 0) {
        perror("Failed to connect");
        exit(1);
    }

    // Loop to read sensor data and send it to the Android device
    while (true) {
        // Read sensor data (replace with your function to read sensor data)
        std::string sensor_data = read_sensor_data();

        // Send the sensor data to the remote Android device over the established connection
        status = write(s, sensor_data.c_str(), sensor_data.size());

        // Check if the data was sent successfully
        if (status < 0) {
            perror("Failed to send data");
            break;
        }

        // Delay between data transmissions (adjust as needed)
        sleep(1);
    }

    // Close the socket after the loop is done
    close(s);
    return 0;
}
