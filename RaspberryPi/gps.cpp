#include <iostream>
#include <gps.h>

int main()
{
    // Initialize the gps structure
    struct gps_data_t gps_data;

    // Open a connection to the GPS daemon
    if (gps_open("localhost", "2947", &gps_data) != 0)
    {
        std::cerr << "Error: Unable to open connection to GPS daemon" << std::endl;
        return 1;
    }

    // Configure the gps structure to receive data
    gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

    // Infinite loop to continuously receive GPS data
    while (true)
    {
        // Wait for new GPS data to be received
        if (gps_waiting(&gps_data, 5000000))
        {
            // Read the GPS data
            if (gps_read(&gps_data) == -1)
            {
                std::cerr << "Error: Unable to read GPS data" << std::endl;
                return 1;
            }

            // Store the GPS coordinates in a buffer
            double latitude = gps_data.fix.latitude;
            double longitude = gps_data.fix.longitude;

            // Print the GPS coordinates
            std::cout << "Latitude: " << latitude << " Longitude: " << longitude << std::endl;
        }
    }

    // Close the connection to the GPS daemon
    gps_close(&gps_data);

    return 0;
}

