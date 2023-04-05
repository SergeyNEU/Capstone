#include "camera.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

Camera::Camera(int width, int height, std::string filename)
    : m_width(width), m_height(height), m_filename(filename) {}

int Camera::captureImage()
{
    pid_t pid_main = fork();

    if (pid_main == -1)
    {
        std::cerr << "Image error: Failed to fork the main process." << std::endl;
        return 1;
    }
    else if (pid_main == 0)
    {
        // In the child process for handling image capture and Python script

        // Generate the command string
        std::string command = "libcamera-still --immediate -n -t1 --width " + std::to_string(m_width) + " --height " + std::to_string(m_height) + " -o " + m_filename + " >/dev/null 2>&1";

        pid_t pid_capture = fork();

        if (pid_capture == -1)
        {
            std::cerr << "Image error: Failed to fork the process." << std::endl;
            exit(1);
        }
        else if (pid_capture == 0)
        {
            // In the child process for image capture
            execl("/bin/sh", "sh", "-c", command.c_str(), (char *)NULL);
            std::cerr << "Image error: Failed to execute command." << std::endl;
            exit(1);
        }
        else
        {
            // In the parent process
            std::cout << "Image Process ID: " << pid_capture;

            // Run the Python script as a child process
            std::string clean_filename = m_filename.substr(9);

            std::cout << "... Done (" << clean_filename << ")" << std::endl;
            std::string python_command = "python3 detect_tflite.py /home/tpp/Desktop/Capstone/RaspberryPi/general/images/" + clean_filename;
            std::string script_directory = "/home/tpp/Desktop/Capstone/ml_new/custom_model_lite"; // Change this to the actual script directory

            pid_t pid_python = fork();

            if (pid_python == -1)
            {
                std::cerr << "Python error: Failed to fork the process." << std::endl;
                exit(1);
            }
            else if (pid_python == 0)
            {
                usleep(200000); // 200 ms
                // In the child process for running the Python script
                chdir(script_directory.c_str()); // Change the working directory to the script's directory
                execl("/bin/sh", "sh", "-c", python_command.c_str(), (char *)NULL);
                std::cerr << "Python error: Failed to execute Python command." << std::endl;
                exit(1);
            }
            else
            {
                // In the parent process
                std::cout << "Python Process ID: " << pid_python << std::endl;
            }

            exit(0); // Ensure this child process exits after completing its tasks
        }
    }
    else
    {
        // In the parent (main) process
        std::cout << "Image and Python handler Process ID: " << pid_main << std::endl;
    }

    return 0;
}