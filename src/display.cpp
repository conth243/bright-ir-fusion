#include "display.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

Display::Display(const std::string& title, int width, int height) {
    windowTitle = title;
    windowWidth = width;
    windowHeight = height;
    
    // 创建窗口
    cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowTitle, width, height);
}

Display::~Display() {
    close();
}

void Display::show(const cv::Mat& frame) {
    if (frame.empty()) {
        return;
    }
    
    // 调整图像大小以适应窗口
    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(windowWidth, windowHeight));
    
    cv::imshow(windowTitle, resizedFrame);
}

void Display::showOverlay(const cv::Mat& frame1, const cv::Mat& frame2, int x1, int y1, double scale1, int x2, int y2, double scale2) {
    // 创建一个空白画布
    cv::Mat canvas(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));
    
    // 处理第一个图像
    if (!frame1.empty()) {
        cv::Mat resizedFrame1;
        int newWidth1 = static_cast<int>(frame1.cols * scale1);
        int newHeight1 = static_cast<int>(frame1.rows * scale1);
        cv::resize(frame1, resizedFrame1, cv::Size(newWidth1, newHeight1));
        
        // 确保图像在画布范围内
        int startX1 = std::max(0, x1);
        int startY1 = std::max(0, y1);
        int endX1 = std::min(windowWidth, x1 + newWidth1);
        int endY1 = std::min(windowHeight, y1 + newHeight1);
        
        if (startX1 < endX1 && startY1 < endY1) {
            cv::Rect roi1(startX1, startY1, endX1 - startX1, endY1 - startY1);
            cv::Rect frameRoi1(startX1 - x1, startY1 - y1, roi1.width, roi1.height);
            resizedFrame1(frameRoi1).copyTo(canvas(roi1));
        }
    }
    
    // 处理第二个图像
    if (!frame2.empty()) {
        cv::Mat resizedFrame2;
        int newWidth2 = static_cast<int>(frame2.cols * scale2);
        int newHeight2 = static_cast<int>(frame2.rows * scale2);
        cv::resize(frame2, resizedFrame2, cv::Size(newWidth2, newHeight2));
        
        // 确保图像在画布范围内
        int startX2 = std::max(0, x2);
        int startY2 = std::max(0, y2);
        int endX2 = std::min(windowWidth, x2 + newWidth2);
        int endY2 = std::min(windowHeight, y2 + newHeight2);
        
        if (startX2 < endX2 && startY2 < endY2) {
            cv::Rect roi2(startX2, startY2, endX2 - startX2, endY2 - startY2);
            cv::Rect frameRoi2(startX2 - x2, startY2 - y2, roi2.width, roi2.height);
            resizedFrame2(frameRoi2).copyTo(canvas(roi2));
        }
    }
    
    cv::imshow(windowTitle, canvas);
}

int Display::waitKey(int delay) {
    return cv::waitKey(delay);
}

void Display::close() {
    if (isOpen()) {
        cv::destroyWindow(windowTitle);
    }
}

bool Display::isOpen() {
    // OpenCV没有直接的方法检查窗口是否打开，这里简单返回true
    // 实际应用中可能需要更复杂的检查
    return true;
}

void Display::setMouseCallback(std::function<void(int, int, int, int)> callback) {
    mouseCallback = callback;
    
    // 设置OpenCV的鼠标回调
    cv::setMouseCallback(windowTitle, [](int event, int x, int y, int flags, void* userdata) {
        Display* display = static_cast<Display*>(userdata);
        if (display->mouseCallback) {
            display->mouseCallback(event, x, y, flags);
        }
    }, this);
}

void Display::getSize(int& width, int& height) {
    width = windowWidth;
    height = windowHeight;
}
