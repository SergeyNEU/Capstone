#ifndef GPS_H
#define GPS_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

using namespace std;

class GPS
{
public:
    GPS();
    vector<string> parse();
    vector<string> processGGAValues(const vector<string>& ggaValues);
    void printGGAValues(const vector<string> &ggaValues);
    bool configureSerialPort();
    int serialPort;

private:
    int openSerialPort(const char *portName);
    void processCoordinate(const string &value, const string &direction, vector<string> &processedData);
    void pushDefaultValue(vector<string> &processedData);
    double rawVal, degrees, minutes, seconds;
};

#endif
