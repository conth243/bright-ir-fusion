#include "camera.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

Camera::Camera() : deviceIndex(-1), width(0), height(0), fps(0) {
}

Camera::~Camera() {
    release();
}

bool Camera::initialize(int index, int width, int height, int fps) {
    deviceIndex = index;
    this->width = width;
    this->height = height;
    this->fps = fps;
    
    // 打开摄像头
    if (!cap.open(index)) {
        return false;
    }
    
    // 设置摄像头参数
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);
    
    // 获取设备名称
    deviceName = "Camera " + std::to_string(index);
    
    return true;
}

bool Camera::capture(cv::Mat& frame) {
    if (!cap.isOpened()) {
        return false;
    }
    
    return cap.read(frame);
}

void Camera::release() {
    if (cap.isOpened()) {
        cap.release();
    }
}

std::string Camera::getDeviceName() {
    return deviceName;
}



std::vector<std::string> enumerateCameras() {
    std::vector<std::string> cameras;
    int index = 0;
    
    while (true) {
        cv::VideoCapture cap(index);
        if (cap.isOpened()) {
            cameras.push_back("Camera " + std::to_string(index));
            cap.release();
            index++;
        } else {
            break;
        }
    }
    
    return cameras;
}

bool isCameraAvailable(int index) {
    cv::VideoCapture cap(index);
    bool available = cap.isOpened();
    cap.release();
    return available;
}

std::string getCameraInfo(int index) {
    cv::VideoCapture cap(index);
    if (!cap.isOpened()) {
        return "Camera not available";
    }
    
    std::string info = "Camera " + std::to_string(index);
    
    // 尝试获取摄像头的一些属性
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);
    
    info += " (" + std::to_string(static_cast<int>(width)) + "x" + 
            std::to_string(static_cast<int>(height)) + ", " + 
            std::to_string(static_cast<int>(fps)) + "fps)";
    
    cap.release();
    return info;
}
