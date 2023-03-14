#include "gps.hpp"

int main()
{
    GPS parser;
    bool serialPortConfiguration = parser.configureSerialPort();

    if (serialPortConfiguration == true)
    {
        printf("Configured serial port\n");
        vector<string> ggaValues;
        for (int x = 0; x < 100; x++)
        {
            sleep(1);
            ggaValues = parser.parse();
            if (!ggaValues.empty())
            {
                parser.printGGAValues(ggaValues);
            }
        }
    }
    else
    {
        printf("Unable to configure serial port\n");
    }

    // Close serial port
    close(parser.serialPort);
    return 0;
}