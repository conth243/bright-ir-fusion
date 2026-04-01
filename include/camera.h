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
    int getWidth();
    int getHeight();
    int getFPS();
    

    
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

// 检测摄像头设备状态
bool isCameraAvailable(int index);

// 获取摄像头设备的详细信息
std::string getCameraInfo(int index);

#endif // CAMERA_H
