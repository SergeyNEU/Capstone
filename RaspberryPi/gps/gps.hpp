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
    void parse();
    int serialPort;

private:
    int openSerialPort(const char *portName);
    void configureSerialPort(int serialPort);
};

#endif