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
    void printGGAValues(const vector<string> &ggaValues);
    bool configureSerialPort();
    int serialPort;

private:
    int openSerialPort(const char *portName);
    double rawVal;
    int degrees;
    double minutes;
};

#endif