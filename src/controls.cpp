#include "controls.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

Controls::Controls() {
}

Controls::~Controls() {
    close();
}

void Controls::createWindow(const std::string& title) {
    windowTitle = title;
    cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
}

void Controls::addTrackbar(const std::string& name, int* value, int max, std::function<void(int)> callback) {
    if (windowTitle.empty()) {
        return;
    }
    
    cv::createTrackbar(name, windowTitle, value, max, [](int val, void* userdata) {
        auto callback = *static_cast<std::function<void(int)>*>(userdata);
        if (callback) {
            callback(val);
        }
    }, &callback);
}

void Controls::addButton(const std::string& name, std::function<void()> callback) {
    if (windowTitle.empty()) {
        return;
    }
    
    // OpenCV没有直接的按钮创建函数，这里使用滑动条模拟
    // 实际应用中可能需要使用更复杂的GUI库
    int* dummy = new int(0);
    buttonCallbacks.push_back(callback);
    
    // 创建一个包含按钮名称和回调的结构体
    struct ButtonData {
        std::string name;
        std::vector<std::function<void()>>* callbacks;
    };
    
    ButtonData* data = new ButtonData{name, &buttonCallbacks};
    
    cv::createTrackbar(name, windowTitle, dummy, 1, [](int val, void* userdata) {
        ButtonData* data = static_cast<ButtonData*>(userdata);
        if (val == 1) {
            // 执行回调
            (*data->callbacks)[data->callbacks->size() - 1]();
            // 重置滑动条
            cv::setTrackbarPos(data->name.c_str(), "控制参数", 0);
        }
    }, data);
}

void Controls::show() {
    if (!windowTitle.empty()) {
        cv::imshow(windowTitle, cv::Mat(1, 1, CV_8UC3, cv::Scalar(200, 200, 200)));
    }
}

void Controls::processEvents() {
    cv::waitKey(1);
}

void Controls::close() {
    if (!windowTitle.empty()) {
        cv::destroyWindow(windowTitle);
    }
}

void Controls::applyPseudoColor(cv::Mat& frame, int colormapType) {
    if (frame.empty()) {
        return;
    }
    
    // 确保输入是单通道图像
    if (frame.channels() > 1) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    }
    
    // 应用伪彩
    cv::applyColorMap(frame, frame, colormapType);
}
