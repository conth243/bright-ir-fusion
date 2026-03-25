#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Camera {
public:
    Camera();
    ~Camera();
    
    // 初始化摄像头
    bool initialize(int index, int width, int height, int fps);
    
    // 采集图像
    bool capture(cv::Mat& frame);
    
    // 释放摄像头
    void release();
    
    // 获取摄像头信息
    std::string getDeviceName();
    
    // 设置摄像头参数
    bool setParameter(const std::string& param, double value);
    
    // 获取摄像头参数
    double getParameter(const std::string& param);
    
private:
    cv::VideoCapture cap;
    int deviceIndex;
    int width;
    int height;
    int fps;
    std::string deviceName;
};

// 枚举所有可用的摄像头
std::vector<std::string> enumerateCameras();

#endif // CAMERA_H
