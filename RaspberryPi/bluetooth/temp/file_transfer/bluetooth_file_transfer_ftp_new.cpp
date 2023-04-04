#include <iostream> // Include the iostream library for input/output operations
#include <sstream>  // Include the stringstream library for creating and manipulating string streams
#include <cstdlib>  // Include the cstdlib library for general-purpose functions, such as system()
#include "bluetooth_file_transfer.hpp" // For main.cpp integration

// Function: transfer_file_over_bluetooth_ftp
// Purpose:  Transfers a file to a remote device over Bluetooth FTP
// Input:    const std::string &bt_addr - Bluetooth address of the remote device
//           int ftp_channel - FTP channel number for the connection
//           const std::string &file_path - Local path of the file to be transferred
// Output:   Returns 'true' if the file transfer was successful, 'false' otherwise
bool transfer_file_over_bluetooth_ftp(const std::string &bt_addr, int ftp_channel, const std::string &file_path)
{
    // Declare a stringstream variable to store the obexftp command string
    std::stringstream command;
    // Populate the command string with the appropriate parameters
    command << "obexftp --nopath --noconn --uuid none --bluetooth " << bt_addr << " --channel " << ftp_channel << " -p " << file_path;

    // Execute the command using the system() function and store the result in an int variable
    int result = std::system(command.str().c_str());

    // Compare the result to 0 and return 'true' if equal, indicating success; otherwise, return 'false'
    return result;
}

/* Commented the main function out of this file so we could integreate it into main.cpp 
int main(int argc, char **argv)
{
    // Check if the correct number of command-line arguments is provided
    if (argc != 4)
    {
        // Print the usage instructions if the number of arguments is incorrect
        std::cout << "Usage: ./bluetooth_file_transfer <bt_addr> <ftp_channel> <file_path>" << std::endl;
        // Return a non-zero exit code to indicate an error
        return 1;
    }

    // Assign the first command-line argument to a string variable for the Bluetooth address
    const std::string bt_addr = argv[1];
    // Convert the second command-line argument to an integer for the FTP channel
    int ftp_channel = std::stoi(argv[2]);
    // Assign the third command-line argument to a string variable for the file path
    const std::string file_path = argv[3];

    // Call the transfer_file_over_bluetooth_ftp function with the provided arguments
    if (transfer_file_over_bluetooth_ftp(bt_addr, ftp_channel, file_path))
    {
        // Print a success message if the file transfer was successful
        std::cout << "File transfer completed successfully." << std::endl;
    }
    else
    {
        // Print an error message if the file transfer failed
        std::cout << "File transfer failed." << std::endl;
    }

    // Return a zero exit code to indicate success
    return 0;
} */
