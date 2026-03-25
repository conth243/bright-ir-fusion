#ifndef CONTROLS_H
#define CONTROLS_H

#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

class Controls {
public:
    Controls();
    ~Controls();
    
    // 创建控制窗口
    void createWindow(const std::string& title);
    
    // 添加滑动条
    void addTrackbar(const std::string& name, int* value, int max, std::function<void(int)> callback = nullptr);
    
    // 添加按钮
    void addButton(const std::string& name, std::function<void()> callback);
    
    // 显示控制窗口
    void show();
    
    // 处理控制事件
    void processEvents();
    
    // 关闭控制窗口
    void close();
    
    // 应用伪彩到红外图像
    void applyPseudoColor(cv::Mat& frame, int colormapType);
    
private:
    std::string windowTitle;
    std::vector<std::function<void()>> buttonCallbacks;
};

#endif // CONTROLS_H
