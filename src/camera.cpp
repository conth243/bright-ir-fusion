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

bool Camera::setParameter(const std::string& param, double value) {
    if (!cap.isOpened()) {
        return false;
    }
    
    if (param == "brightness") {
        return cap.set(cv::CAP_PROP_BRIGHTNESS, value);
    } else if (param == "contrast") {
        return cap.set(cv::CAP_PROP_CONTRAST, value);
    } else if (param == "saturation") {
        return cap.set(cv::CAP_PROP_SATURATION, value);
    } else if (param == "exposure") {
        return cap.set(cv::CAP_PROP_EXPOSURE, value);
    } else if (param == "gain") {
        return cap.set(cv::CAP_PROP_GAIN, value);
    }
    
    return false;
}

double Camera::getParameter(const std::string& param) {
    if (!cap.isOpened()) {
        return -1.0;
    }
    
    if (param == "brightness") {
        return cap.get(cv::CAP_PROP_BRIGHTNESS);
    } else if (param == "contrast") {
        return cap.get(cv::CAP_PROP_CONTRAST);
    } else if (param == "saturation") {
        return cap.get(cv::CAP_PROP_SATURATION);
    } else if (param == "exposure") {
        return cap.get(cv::CAP_PROP_EXPOSURE);
    } else if (param == "gain") {
        return cap.get(cv::CAP_PROP_GAIN);
    }
    
    return -1.0;
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
