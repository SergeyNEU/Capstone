#include "gps.hpp"

#define MAX_BUFFER_SIZE 2048

GPS::GPS()
{

}

// Function to open serial port
int GPS::openSerialPort(const char *portName)
{
    serialPort = open(portName, O_RDONLY | O_NOCTTY | O_SYNC);
    if (serialPort == -1)
    {
        perror("Error: Unable to open serial port");
    }
    return serialPort;
}

// Function to configure serial port
bool GPS::configureSerialPort()
{
    int internal_serialPort = openSerialPort("/dev/ttyUSB0");

    if (internal_serialPort == -1){
        return false;
    }

    struct termios serialConfig;
    memset(&serialConfig, 0, sizeof(serialConfig)); // Set baud rate to 9600
    cfsetispeed(&serialConfig, B4800);
    cfsetospeed(&serialConfig, B4800);
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
        return false;
    }

    return true;
}

// Function to parse NMEA message and return GGA values as vector
vector<string> GPS::parse()
{
    char buffer[MAX_BUFFER_SIZE];
    int bytesRead = 0;
    // Read bytes from serial port
    bytesRead = read(serialPort, buffer, MAX_BUFFER_SIZE);
    if (bytesRead == -1)
    {
        perror("Error: Unable to read from serial port");
        exit(EXIT_FAILURE);
    }

    // Parse NMEA message
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

        return ggaValues;
    }

    return {}; // empty vector if no GGA message found
}

// Function to print out GGA values with correct formatting
void GPS::printGGAValues(const vector<string> &ggaValues)
{
    if (ggaValues.size() < 15)
    {
        cout << "Invalid GGA sentence: insufficient data" << endl;
        return;
    }

    cout << "Time (UTC): " << ggaValues[1] << " | ";

    if(ggaValues[2] == ""){
        cout << "Lat: " << 0 << " deg " << 0 << "' " << 0 << "'' N/A" << " | ";
    } else {
        rawVal = std::stod(ggaValues[2]);
        degrees = static_cast<int>(rawVal / 100);
        minutes = static_cast<int>(rawVal - (degrees * 100));
        seconds = (rawVal - (degrees * 100) - minutes) * 60;

        //DMS format, DEGREES MINUTES SECONDS
        cout << "Lat: " << degrees << " deg " << minutes << "' " << seconds << "'' " << ggaValues[3] << " | ";
    }

    if(ggaValues[4] == ""){
        cout << "Long: " << 0 << " deg " << 0 << "' " << 0 << "'' N/A" << " | ";
    } else {
        rawVal = std::stod(ggaValues[4]);
        degrees = static_cast<int>(rawVal / 100);
        minutes = static_cast<int>(rawVal - (degrees * 100));
        seconds = (rawVal - (degrees * 100) - minutes) * 60;

        //DMS format, DEGREES MINUTES SECONDS
        cout << "Long: " << degrees << " deg " << minutes << "' " << seconds << "'' " << ggaValues[5] << " | ";
    }

    //Fix Quality
    cout << "Fix: ";
    
    switch (stoi(ggaValues[6]))
    {
    case 0:
        cout << "Not Valid";
        break;
    case 1:
        cout << "Valid";
        break;
    // case 2:
    //     cout << "Differential GPS fix (DGNSS), SBAS, OmniSTAR VBS, Beacon, RTX in GVBS mode";
    //     break;
    // case 3:
    //     cout << "Not applicable";
    //     break;
    // case 4:
    //     cout << "RTK Fixed, xFill";
    //     break;
    // case 5:
    //     cout << "RTK Float, OmniSTAR XP/HP, Location RTK, RTX";
    //     break;
    // case 6:
    //     cout << "INS Dead reckoning";
    //     break;
    default:
        cout << stoi(ggaValues[6]);
    }
    cout << " | ";

    //Number of Satellites
    cout << "# Sat: " << std::stod(ggaValues[7]) << " | ";

    //Horizontal Dilution of Precision (
    cout << "HDOP: " << ggaValues[8] << " | ";
    cout << "Alt: " << ggaValues[9] << " " << ggaValues[10] << " | ";

    //Geoidal Separation
    cout << "G.S: " << ggaValues[11] << " " << ggaValues[12] << " | ";

    //Age of Differential GPS Data
    cout << "Diff Age: " << ggaValues[13] << " | ";

    //Differential Reference Station ID
    cout << "Ref ID: " << ggaValues[14] << endl;
}