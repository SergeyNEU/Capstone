#include "gps.hpp"

#define MAX_BUFFER_SIZE 1024

GPS::GPS()
{
    serialPort = openSerialPort("/dev/ttyUSB0");
    configureSerialPort(serialPort);
}

// Function to open serial port
int GPS::openSerialPort(const char *portName)
{
    int serialPort = open(portName, O_RDONLY | O_NOCTTY | O_SYNC);
    if (serialPort == -1)
    {
        perror("Error: Unable to open serial port");
        exit(EXIT_FAILURE);
    }
    return serialPort;
}

// Function to configure serial port
void GPS::configureSerialPort(int serialPort)
{
    struct termios serialConfig;
    memset(&serialConfig, 0, sizeof(serialConfig)); // Set baud rate to 9600
    cfsetispeed(&serialConfig, B9600);
    cfsetospeed(&serialConfig, B9600);
    // Set character size to 8 bits
    serialConfig.c_cflag &= ~CSIZE;
    serialConfig.c_cflag |= CS8;
    // Set parity to none
    serialConfig.c_cflag &= ~PARENB;

    // Set stop bits to 1
    serialConfig.c_cflag &= ~CSTOPB;

    // Enable receiver
    serialConfig.c_cflag |= CREAD;

    // Set blocking mode
    serialConfig.c_cc[VMIN] = 1;
    serialConfig.c_cc[VTIME] = 0;

    // Apply settings
    if (tcsetattr(serialPort, TCSANOW, &serialConfig) < 0)
    {
        perror("Error: Unable to configure serial port");
        exit(EXIT_FAILURE);
    }
}

// Function to parse NMEA message
void GPS::parse()
{
    char buffer[MAX_BUFFER_SIZE];
    int bytesRead = 0;
    // Read bytes from serial port
    bytesRead = read(serialPort, buffer, MAX_BUFFER_SIZE);
    if (bytesRead == -1)
    {
        perror("Error: Unable to read from serial port");
        exit(EXIT_FAILURE);
    } // Parse NMEA message
    string nmeaMessage(buffer, bytesRead);

    size_t ggaPos = nmeaMessage.find("$GPGGA");
    if (ggaPos != string::npos)
    { // found GGA message
        // Extract GGA message
        string ggaString = nmeaMessage.substr(ggaPos);
        size_t endPos = ggaString.find("\r\n"); // find end of message
        if (endPos != string::npos)
        {
            ggaString = ggaString.substr(0, endPos); // remove anything after end of message
        }

        // Split GGA message into comma-separated values
        vector<string> ggaValues;
        size_t start = 0, end;
        while ((end = ggaString.find(",", start)) != string::npos)
        {
            ggaValues.push_back(ggaString.substr(start, end - start));
            start = end + 1;
        }
        ggaValues.push_back(ggaString.substr(start)); // add last value
        // Print out GGA values and their meaning
        cout << "GGA Message:" << endl;
        cout << "  Time (UTC): " << ggaValues[1] << endl;
        cout << "  Latitude: " << ggaValues[2] << " " << ggaValues[3] << endl;
        cout << "  Longitude: " << ggaValues[4] << " " << ggaValues[5] << endl;
        cout << "  Fix Quality: " << ggaValues[6] << " (";
        switch (stoi(ggaValues[6]))
        {
        case 0:
            cout << "Fix not valid";
            break;
        case 1:
            cout << "GPS fix";
            break;
        case 2:
            cout << "Differential GPS fix (DGNSS), SBAS, OmniSTAR VBS, Beacon, RTX in GVBS mode";
            break;
        case 3:
            cout << "Not applicable";
            break;
        case 4:
            cout << "RTK Fixed, xFill";
            break;
        case 5:
            cout << "RTK Float, OmniSTAR XP/HP, Location RTK, RTX";
            break;
        case 6:
            cout << "INS Dead reckoning";
            break;
        default:
            cout << "Unknown";
        }
        cout << ")" << endl;
        cout << "  Number of Satellites: " << ggaValues[7] << endl;
        cout << "  Horizontal Dilution of Precision (HDOP): " << ggaValues[8] << endl;
        cout << "  Altitude: " << ggaValues[9] << " " << ggaValues[10] << endl;
        cout << "  Geoidal Separation: " << ggaValues[11] << " " << ggaValues[12] << endl;
        cout << "  Age of Differential GPS Data: " << ggaValues[13] << endl;
        cout << "  Differential Reference Station ID: " << ggaValues[14] << endl;
    }
}