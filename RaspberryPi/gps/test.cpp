#include "gps.hpp"

int main()
{
    GPS parser;
    parser.parse();
    
    // Close serial port
    close(parser.serialPort);
    return 0;
}