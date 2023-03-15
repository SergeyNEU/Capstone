#include "gps.hpp"

int main()
{
    GPS parser;
    bool serialPortConfiguration = parser.configureSerialPort();

    if (serialPortConfiguration == true)
    {
        printf("Configured serial port (/dev/ttyUSB0).\n");
        vector<string> ggaValues;
        for (int x = 0; x < 100; x++)
        {
            sleep(1);
            ggaValues = parser.parse();
            if (!ggaValues.empty())
            {
                parser.printGGAValues(ggaValues);
            }
            else
            {
                printf("No serial port output.\n");
            }
        }
    }
    else
    {
        printf("Unable to configure serial port (/dev/ttyUSB0).\n");
    }

    // Close serial port
    close(parser.serialPort);
    return 0;
}