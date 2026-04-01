#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <functional>
#include <vector>
#include <sstream>
#include "camera.h"

struct Button {
    std::string label;
    cv::Rect rect;
    std::function<void()> callback;
    bool isHovered;
    bool isPressed;
    cv::Scalar normalColor;
    cv::Scalar hoverColor;
    cv::Scalar pressedColor;
};

class UIManager {
public:
    UIManager();
    ~UIManager();
    
    bool initialize(const std::string& windowName, int windowWidth, int windowHeight);
    void setDisplayImage(const cv::Mat& image);
    void render();
    void handleMouseEvent(int event, int x, int y, int flags);
    void handleKeyEvent(int key);
    void showAboutDialog();
    void close();
    bool isRunning() const;
    
    void setPowerCallback(std::function<void()> callback);
    void setMenuCallback(std::function<void()> callback);
    void setRangeCallback(std::function<void()> callback);
    void setZoomCallback(std::function<void()> callback);
    void setCaptureCallback(std::function<void()> callback);
    void setPseudoColorCallback(std::function<void()> callback);
    
    // 摄像头检测和选择相关方法
    void showCameraDetectionDialog(const std::vector<std::string>& cameras);
    void showCameraDetectionDialogWithInfo(const std::vector<CameraInfo>& cameraInfos);
    void setCameraSelectedCallback(std::function<void(int)> callback);
    void setDeviceInfo(const std::string& deviceInfo);
    
private:
    std::string windowName_;
    int windowWidth_;
    int windowHeight_;
    bool isRunning_;
    
    cv::Rect displayRect_;
    cv::Mat displayImage_;
    cv::Mat background_;
    
    std::vector<Button> leftButtons_;
    std::vector<Button> rightButtons_;
    
    bool showAbout_;
    cv::Rect aboutDialogRect_;
    
    std::string versionInfo_;
    std::string deviceInfo_;
    
    void initializeButtons();
    void drawButton(cv::Mat& canvas, Button& button);
    void drawAboutDialog(cv::Mat& canvas);
    bool isPointInRect(const cv::Point& point, const cv::Rect& rect);
    
    std::function<void()> powerCallback_;
    std::function<void()> menuCallback_;
    std::function<void()> rangeCallback_;
    std::function<void()> zoomCallback_;
    std::function<void()> captureCallback_;
    std::function<void()> pseudoColorCallback_;
    std::function<void(int)> cameraSelectedCallback_;
    
    // 摄像头检测对话框相关
    bool showCameraDialog_;
    std::vector<std::string> detectedCameras_;
    std::vector<CameraInfo> detectedCameraInfos_;
    bool showCameraInfo_;
    int selectedCameraIndex_;
    cv::Rect cameraDialogRect_;
    cv::Rect cameraListRect_;
    cv::Rect confirmButtonRect_;
    
    void defaultPowerCallback();
    void defaultMenuCallback();
    void defaultRangeCallback();
    void defaultZoomCallback();
    void defaultCaptureCallback();
    void defaultPseudoColorCallback();
    void drawCameraDetectionDialog(cv::Mat& canvas);
};

#endif // UI_MANAGER_H