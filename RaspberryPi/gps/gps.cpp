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

    if (internal_serialPort == -1)
    {
        return false;
    }

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
        return {};
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

// Add the helper function implementations
void GPS::processCoordinate(const string &value, const string &direction, vector<string> &processedData)
{
    rawVal = std::stod(value);
    degrees = static_cast<int>(rawVal / 100);
    minutes = static_cast<int>(rawVal - (degrees * 100));
    seconds = (rawVal - (degrees * 100) - minutes) * 60;

    // DMS format, DEGREES MINUTES SECONDS
    processedData.push_back(to_string(degrees));
    processedData.push_back(to_string(minutes));
    processedData.push_back(to_string(seconds));
    processedData.push_back(direction);
}

void GPS::pushDefaultValue(vector<string> &processedData)
{
    processedData.push_back("0");
    processedData.push_back("0");
    processedData.push_back("0");
    processedData.push_back("N/A");
}

// Updated GPS::processGGAValues function
vector<string> GPS::processGGAValues(const vector<string> &ggaValues)
{
    vector<string> processedData;

    if (ggaValues.empty() || ggaValues.size() < 15)
    {
        cout << "Invalid GGA sentence: insufficient data" << endl;

        // Fill the processedData vector with default values
        pushDefaultValue(processedData); // Latitude
        pushDefaultValue(processedData); // Longitude

        for (size_t i = 2; i < 15; ++i)
        {
            processedData.push_back("N/A");
        }
        
        return processedData;
    }

    processedData.push_back(ggaValues[1]);

    // Process latitude value
    if (ggaValues[2].empty() || ggaValues[2] == "N/A" || ggaValues[3] == "N/A")
    {
        pushDefaultValue(processedData);
    }
    else
    {
        try
        {
            processCoordinate(ggaValues[2], ggaValues[3], processedData);
        }
        catch (const std::exception &e)
        {
            cout << "Error processing latitude value: " << e.what() << endl;
            cout << "Value: " << ggaValues[2] << endl;
        }
    }

    // Process longitude value
    if (ggaValues[4].empty() || ggaValues[4] == "N/A" || ggaValues[5] == "N/A")
    {
        pushDefaultValue(processedData);
    }
    else
    {
        try
        {
            processCoordinate(ggaValues[4], ggaValues[5], processedData);
        }
        catch (const std::exception &e)
        {
            cout << "Error processing longitude value: " << e.what() << endl;
            cout << "Value: " << ggaValues[4] << endl;
        }
    }

    // Process remaining values
    for (size_t i = 6; i < 15; ++i)
    {
        if (ggaValues[i].empty())
        {
            processedData.push_back("N/A");
        }
        else
        {
            if ((i == 9) || (i == 11))
            {
                processedData.push_back(ggaValues[i] + " " + ggaValues[i + 1]);
                ++i;
            }
            else
            {
                processedData.push_back(ggaValues[i]);
            }
        }
    }

    return processedData;
}

// Function to print out GGA values
void GPS::printGGAValues(const vector<string> &ggaValues)
{
    vector<string> processedData = ggaValues;

    cout << "Time (UTC): " << processedData[0] << " | ";
    cout << "Lat: " << processedData[1] << " deg " << processedData[2] << "' " << processedData[3] << "'' " << processedData[4] << " | ";
    cout << "Long " << processedData[5] << " deg " << processedData[6] << "' " << processedData[7] << "'' " << processedData[8] << " | ";
    cout << "Fix: " << processedData[9] << " | ";
    cout << "# Sat: " << processedData[10] << " | ";
    cout << "HDOP: " << processedData[11] << " | ";
    cout << "Alt: " << processedData[12] << " " << processedData[13] << " | ";
    cout << "G.S: " << processedData[14] << " " << processedData[15] << " | ";
    cout << "Diff Age: " << processedData[16] << " | ";
    cout << "Ref ID: " << processedData[17] << endl;
}
