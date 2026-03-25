#ifndef DISPLAY_H
#define DISPLAY_H

#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

class Display {
public:
    Display(const std::string& title, int width, int height);
    ~Display();
    
    // 显示图像
    void show(const cv::Mat& frame);
    
    // 显示叠加图像
    void showOverlay(const cv::Mat& frame1, const cv::Mat& frame2, int x1, int y1, double scale1, int x2, int y2, double scale2);
    
    // 等待按键
    int waitKey(int delay = 1);
    
    // 关闭窗口
    void close();
    
    // 检查窗口是否打开
    bool isOpen();
    
    // 设置鼠标回调函数
    void setMouseCallback(std::function<void(int, int, int, int)> callback);
    
    // 获取窗口尺寸
    void getSize(int& width, int& height);
    
private:
    std::string windowTitle;
    int windowWidth;
    int windowHeight;
    std::function<void(int, int, int, int)> mouseCallback;
};

#endif // DISPLAY_H
