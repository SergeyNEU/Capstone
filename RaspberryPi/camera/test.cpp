#include "camera.hpp"

int main() {
    int width = 4608;
    int height = 2592;
    std::string filename = "./image.jpg";

    Camera camera(width, height, filename);
    camera.captureImage();

    return 0;
}