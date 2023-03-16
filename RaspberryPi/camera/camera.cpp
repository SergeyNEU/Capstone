#include "camera.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

Camera::Camera(int width, int height, std::string filename)
    : m_width(width), m_height(height), m_filename(filename) {}

int Camera::captureImage()
{
    // Generate the command string
    std::string command = "libcamera-still --immediate -n -t1 --width " + std::to_string(m_width) + " --height " + std::to_string(m_height) + " -o " + m_filename + " >/dev/null 2>&1";

    // Execute the command and wait for it to complete
    int status = std::system(command.c_str());
    if (status != 0)
    {
        std::cerr << "Error: Failed to capture image." << std::endl;
        return 1;
    }

    std::cout << "Image saved to " << m_filename << std::endl;

    return 0;
}