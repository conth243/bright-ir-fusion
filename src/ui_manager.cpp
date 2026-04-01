#include "ui_manager.h"
#include <opencv2/opencv.hpp>
#include <iostream>

UIManager::UIManager() 
    : windowWidth_(1024)
    , windowHeight_(768)
    , isRunning_(false)
    , showAbout_(false)
    , showCameraDialog_(false)
    , showCameraInfo_(false)
    , selectedCameraIndex_(0)
    , versionInfo_("Bright IR Fusion v1.1")
    , deviceInfo_("No camera found") {
}

UIManager::~UIManager() {
    close();
}

bool UIManager::initialize(const std::string& windowName, int windowWidth, int windowHeight) {
    windowName_ = windowName;
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
    
    cv::namedWindow(windowName_, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowName_, windowWidth_, windowHeight_);
    
    int displayX = (windowWidth_ - 640) / 2;
    int displayY = (windowHeight_ - 512) / 2;
    displayRect_ = cv::Rect(displayX, displayY, 640, 512);
    
    background_ = cv::Mat(windowHeight_, windowWidth_, CV_8UC3, cv::Scalar(50, 50, 50));
    
    initializeButtons();
    
    cv::setMouseCallback(windowName_, [](int event, int x, int y, int flags, void* userdata) {
        UIManager* ui = static_cast<UIManager*>(userdata);
        ui->handleMouseEvent(event, x, y, flags);
    }, this);
    
    isRunning_ = true;
    return true;
}

void UIManager::initializeButtons() {
    int buttonWidth = 80;
    int buttonHeight = 40;
    int buttonSpacing = 20;
    int leftButtonX = 50;
    int rightButtonX = windowWidth_ - 50 - buttonWidth;
    int startY = (windowHeight_ - (3 * buttonHeight + 2 * buttonSpacing)) / 2;
    
    Button powerBtn;
    powerBtn.label = "Power";
    powerBtn.rect = cv::Rect(leftButtonX, startY, buttonWidth, buttonHeight);
    powerBtn.callback = [this]() { defaultPowerCallback(); };
    powerBtn.isHovered = false;
    powerBtn.isPressed = false;
    powerBtn.normalColor = cv::Scalar(100, 100, 100);
    powerBtn.hoverColor = cv::Scalar(150, 150, 150);
    powerBtn.pressedColor = cv::Scalar(80, 80, 80);
    leftButtons_.push_back(powerBtn);
    
    Button menuBtn;
    menuBtn.label = "Menu";
    menuBtn.rect = cv::Rect(leftButtonX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
    menuBtn.callback = [this]() { defaultMenuCallback(); };
    menuBtn.isHovered = false;
    menuBtn.isPressed = false;
    menuBtn.normalColor = cv::Scalar(100, 100, 100);
    menuBtn.hoverColor = cv::Scalar(150, 150, 150);
    menuBtn.pressedColor = cv::Scalar(80, 80, 80);
    leftButtons_.push_back(menuBtn);
    
    Button rangeBtn;
    rangeBtn.label = "Range";
    rangeBtn.rect = cv::Rect(leftButtonX, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight);
    rangeBtn.callback = [this]() { defaultRangeCallback(); };
    rangeBtn.isHovered = false;
    rangeBtn.isPressed = false;
    rangeBtn.normalColor = cv::Scalar(100, 100, 100);
    rangeBtn.hoverColor = cv::Scalar(150, 150, 150);
    rangeBtn.pressedColor = cv::Scalar(80, 80, 80);
    leftButtons_.push_back(rangeBtn);
    
    Button zoomBtn;
    zoomBtn.label = "Zoom";
    zoomBtn.rect = cv::Rect(rightButtonX, startY, buttonWidth, buttonHeight);
    zoomBtn.callback = [this]() { defaultZoomCallback(); };
    zoomBtn.isHovered = false;
    zoomBtn.isPressed = false;
    zoomBtn.normalColor = cv::Scalar(100, 100, 100);
    zoomBtn.hoverColor = cv::Scalar(150, 150, 150);
    zoomBtn.pressedColor = cv::Scalar(80, 80, 80);
    rightButtons_.push_back(zoomBtn);
    
    Button captureBtn;
    captureBtn.label = "Capture";
    captureBtn.rect = cv::Rect(rightButtonX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
    captureBtn.callback = [this]() { defaultCaptureCallback(); };
    captureBtn.isHovered = false;
    captureBtn.isPressed = false;
    captureBtn.normalColor = cv::Scalar(100, 100, 100);
    captureBtn.hoverColor = cv::Scalar(150, 150, 150);
    captureBtn.pressedColor = cv::Scalar(80, 80, 80);
    rightButtons_.push_back(captureBtn);
    
    Button pseudoBtn;
    pseudoBtn.label = "Pseudo";
    pseudoBtn.rect = cv::Rect(rightButtonX, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight);
    pseudoBtn.callback = [this]() { defaultPseudoColorCallback(); };
    pseudoBtn.isHovered = false;
    pseudoBtn.isPressed = false;
    pseudoBtn.normalColor = cv::Scalar(100, 100, 100);
    pseudoBtn.hoverColor = cv::Scalar(150, 150, 150);
    pseudoBtn.pressedColor = cv::Scalar(80, 80, 80);
    rightButtons_.push_back(pseudoBtn);
}

void UIManager::setDisplayImage(const cv::Mat& image) {
    if (!image.empty()) {
        cv::resize(image, displayImage_, cv::Size(displayRect_.width, displayRect_.height));
    }
}

void UIManager::render() {
    cv::Mat canvas = background_.clone();
    
    cv::rectangle(canvas, displayRect_, cv::Scalar(200, 200, 200), 2);
    
    if (!displayImage_.empty()) {
        displayImage_.copyTo(canvas(displayRect_));
    } else {
        cv::Mat placeholder(displayRect_.height, displayRect_.width, CV_8UC3, cv::Scalar(30, 30, 30));
        cv::putText(placeholder, "No Image", cv::Point(displayRect_.width/2 - 50, displayRect_.height/2),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(150, 150, 150), 2);
        placeholder.copyTo(canvas(displayRect_));
    }
    
    for (auto& button : leftButtons_) {
        drawButton(canvas, button);
    }
    
    for (auto& button : rightButtons_) {
        drawButton(canvas, button);
    }
    
    if (showAbout_) {
        drawAboutDialog(canvas);
    }
    
    if (showCameraDialog_) {
        drawCameraDetectionDialog(canvas);
    }
    
    cv::imshow(windowName_, canvas);
}

void UIManager::drawButton(cv::Mat& canvas, Button& button) {
    cv::Scalar color;
    if (button.isPressed) {
        color = button.pressedColor;
    } else if (button.isHovered) {
        color = button.hoverColor;
    } else {
        color = button.normalColor;
    }
    
    cv::rectangle(canvas, button.rect, color, -1);
    cv::rectangle(canvas, button.rect, cv::Scalar(200, 200, 200), 1);
    
    int baseline;
    cv::Size textSize = cv::getTextSize(button.label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
    int textX = button.rect.x + (button.rect.width - textSize.width) / 2;
    int textY = button.rect.y + (button.rect.height + textSize.height) / 2;
    cv::putText(canvas, button.label, cv::Point(textX, textY),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
}

void UIManager::drawAboutDialog(cv::Mat& canvas) {
    int dialogWidth = 450;
    int dialogHeight = 350;
    int dialogX = (windowWidth_ - dialogWidth) / 2;
    int dialogY = (windowHeight_ - dialogHeight) / 2;
    aboutDialogRect_ = cv::Rect(dialogX, dialogY, dialogWidth, dialogHeight);
    
    cv::rectangle(canvas, aboutDialogRect_, cv::Scalar(80, 80, 80), -1);
    cv::rectangle(canvas, aboutDialogRect_, cv::Scalar(200, 200, 200), 2);
    
    cv::putText(canvas, "About", cv::Point(dialogX + 20, dialogY + 40),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    
    std::istringstream iss(versionInfo_);
    std::string line;
    int lineY = dialogY + 80;
    while (std::getline(iss, line)) {
        cv::putText(canvas, line, cv::Point(dialogX + 20, lineY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
        lineY += 30;
    }
    
    cv::line(canvas, cv::Point(dialogX + 20, lineY + 10), cv::Point(dialogX + dialogWidth - 20, lineY + 10), cv::Scalar(150, 150, 150), 1);
    
    cv::putText(canvas, "Device Information", cv::Point(dialogX + 20, lineY + 40),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 1);
    
    std::istringstream deviceIss(deviceInfo_);
    int deviceLineY = lineY + 70;
    while (std::getline(deviceIss, line)) {
        cv::putText(canvas, line, cv::Point(dialogX + 20, deviceLineY),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
        deviceLineY += 25;
    }
    
    cv::putText(canvas, "Press ESC or click outside to close", cv::Point(dialogX + 20, dialogY + dialogHeight - 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(150, 150, 150), 1);
}

void UIManager::drawCameraDetectionDialog(cv::Mat& canvas) {
    int dialogWidth = 450;
    int dialogHeight = detectedCameras_.empty() ? 200 : 350;
    int dialogX = (windowWidth_ - dialogWidth) / 2;
    int dialogY = (windowHeight_ - dialogHeight) / 2;
    cameraDialogRect_ = cv::Rect(dialogX, dialogY, dialogWidth, dialogHeight);
    
    cv::rectangle(canvas, cameraDialogRect_, cv::Scalar(80, 80, 80), -1);
    cv::rectangle(canvas, cameraDialogRect_, cv::Scalar(200, 200, 200), 2);
    
    cv::putText(canvas, "Camera Detection", cv::Point(dialogX + 20, dialogY + 40),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    
    if (detectedCameras_.empty()) {
        cv::putText(canvas, "No cameras detected!", cv::Point(dialogX + 20, dialogY + 80),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 100, 100), 1);
        cv::putText(canvas, "Please connect a UVC camera and try again.", cv::Point(dialogX + 20, dialogY + 120),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
    } else if (detectedCameras_.size() == 1) {
        cv::putText(canvas, "Camera found:", cv::Point(dialogX + 20, dialogY + 80),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(200, 200, 200), 1);
        cv::putText(canvas, detectedCameras_[0], cv::Point(dialogX + 20, dialogY + 120),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(100, 255, 100), 1);
        
        if (showCameraInfo_ && !detectedCameraInfos_.empty()) {
            const auto& info = detectedCameraInfos_[0];
            std::string resolution = "Resolution: " + std::to_string(info.width) + "x" + std::to_string(info.height);
            std::string fps = "Frame Rate: " + std::to_string(info.fps) + "fps";
            
            cv::putText(canvas, resolution, cv::Point(dialogX + 20, dialogY + 160),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
            cv::putText(canvas, fps, cv::Point(dialogX + 20, dialogY + 190),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
        }
    } else {
        cv::putText(canvas, "Multiple cameras found:", cv::Point(dialogX + 20, dialogY + 80),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(200, 200, 200), 1);
        
        int listWidth = dialogWidth - 40;
        int listHeight = 150;
        int listX = dialogX + 20;
        int listY = dialogY + 110;
        cameraListRect_ = cv::Rect(listX, listY, listWidth, listHeight);
        
        cv::rectangle(canvas, cameraListRect_, cv::Scalar(100, 100, 100), -1);
        cv::rectangle(canvas, cameraListRect_, cv::Scalar(200, 200, 200), 1);
        
        int itemHeight = listHeight / detectedCameras_.size();
        for (size_t i = 0; i < detectedCameras_.size(); i++) {
            cv::Rect itemRect(listX, listY + i * itemHeight, listWidth, itemHeight);
            if (static_cast<int>(i) == selectedCameraIndex_) {
                cv::rectangle(canvas, itemRect, cv::Scalar(150, 150, 255), -1);
            }
            cv::putText(canvas, detectedCameras_[i], cv::Point(listX + 10, listY + i * itemHeight + itemHeight / 2 + 5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        }
        
        if (showCameraInfo_ && !detectedCameraInfos_.empty() && selectedCameraIndex_ < detectedCameraInfos_.size()) {
            const auto& info = detectedCameraInfos_[selectedCameraIndex_];
            std::string resolution = "Resolution: " + std::to_string(info.width) + "x" + std::to_string(info.height);
            std::string fps = "Frame Rate: " + std::to_string(info.fps) + "fps";
            
            cv::putText(canvas, "Selected camera info:", cv::Point(dialogX + 20, dialogY + 270),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
            cv::putText(canvas, resolution, cv::Point(dialogX + 20, dialogY + 300),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
            cv::putText(canvas, fps, cv::Point(dialogX + 20, dialogY + 320),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
        }
    }
    
    int buttonWidth = 100;
    int buttonHeight = 30;
    int buttonX = dialogX + (dialogWidth - buttonWidth) / 2;
    int buttonY = dialogY + dialogHeight - 50;
    confirmButtonRect_ = cv::Rect(buttonX, buttonY, buttonWidth, buttonHeight);
    
    cv::rectangle(canvas, confirmButtonRect_, cv::Scalar(100, 150, 100), -1);
    cv::rectangle(canvas, confirmButtonRect_, cv::Scalar(200, 200, 200), 1);
    cv::putText(canvas, "OK", cv::Point(buttonX + 40, buttonY + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 1);
}

void UIManager::handleMouseEvent(int event, int x, int y, int flags) {
    cv::Point point(x, y);
    
    if (showAbout_) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            if (!isPointInRect(point, aboutDialogRect_)) {
                showAbout_ = false;
                return;
            }
        }
        return;
    }
    
    if (showCameraDialog_) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            if (isPointInRect(point, confirmButtonRect_)) {
                if (cameraSelectedCallback_ && !detectedCameras_.empty()) {
                    cameraSelectedCallback_(selectedCameraIndex_);
                }
                showCameraDialog_ = false;
                return;
            }
            
            if (isPointInRect(point, cameraListRect_) && detectedCameras_.size() > 1) {
                int itemHeight = cameraListRect_.height / detectedCameras_.size();
                int clickedIndex = (point.y - cameraListRect_.y) / itemHeight;
                if (clickedIndex >= 0 && clickedIndex < static_cast<int>(detectedCameras_.size())) {
                    selectedCameraIndex_ = clickedIndex;
                }
            }
            
            if (!isPointInRect(point, cameraDialogRect_)) {
                showCameraDialog_ = false;
            }
            return;
        }
        return;
    }
    
    for (auto& button : leftButtons_) {
        if (isPointInRect(point, button.rect)) {
            if (event == cv::EVENT_MOUSEMOVE) {
                button.isHovered = true;
            } else if (event == cv::EVENT_LBUTTONDOWN) {
                button.isPressed = true;
            } else if (event == cv::EVENT_LBUTTONUP) {
                button.isPressed = false;
                if (button.callback) {
                    button.callback();
                }
            }
        } else {
            button.isHovered = false;
            button.isPressed = false;
        }
    }
    
    for (auto& button : rightButtons_) {
        if (isPointInRect(point, button.rect)) {
            if (event == cv::EVENT_MOUSEMOVE) {
                button.isHovered = true;
            } else if (event == cv::EVENT_LBUTTONDOWN) {
                button.isPressed = true;
            } else if (event == cv::EVENT_LBUTTONUP) {
                button.isPressed = false;
                if (button.callback) {
                    button.callback();
                }
            }
        } else {
            button.isHovered = false;
            button.isPressed = false;
        }
    }
}

void UIManager::handleKeyEvent(int key) {
    if (key == 27) {
        if (showAbout_) {
            showAbout_ = false;
        } else if (showCameraDialog_) {
            showCameraDialog_ = false;
        } else {
            isRunning_ = false;
        }
    }
}

void UIManager::showAboutDialog() {
    showAbout_ = true;
}

void UIManager::close() {
    if (isRunning_) {
        cv::destroyWindow(windowName_);
        isRunning_ = false;
    }
}

bool UIManager::isRunning() const {
    return isRunning_;
}

bool UIManager::isPointInRect(const cv::Point& point, const cv::Rect& rect) {
    return point.x >= rect.x && point.x < rect.x + rect.width &&
           point.y >= rect.y && point.y < rect.y + rect.height;
}

void UIManager::setPowerCallback(std::function<void()> callback) {
    powerCallback_ = callback;
    if (!leftButtons_.empty()) {
        leftButtons_[0].callback = callback;
    }
}

void UIManager::setMenuCallback(std::function<void()> callback) {
    menuCallback_ = callback;
    if (leftButtons_.size() > 1) {
        leftButtons_[1].callback = callback;
    }
}

void UIManager::setRangeCallback(std::function<void()> callback) {
    rangeCallback_ = callback;
    if (leftButtons_.size() > 2) {
        leftButtons_[2].callback = callback;
    }
}

void UIManager::setZoomCallback(std::function<void()> callback) {
    zoomCallback_ = callback;
    if (!rightButtons_.empty()) {
        rightButtons_[0].callback = callback;
    }
}

void UIManager::setCaptureCallback(std::function<void()> callback) {
    captureCallback_ = callback;
    if (rightButtons_.size() > 1) {
        rightButtons_[1].callback = callback;
    }
}

void UIManager::setPseudoColorCallback(std::function<void()> callback) {
    pseudoColorCallback_ = callback;
    if (rightButtons_.size() > 2) {
        rightButtons_[2].callback = callback;
    }
}

void UIManager::showCameraDetectionDialog(const std::vector<std::string>& cameras) {
    detectedCameras_ = cameras;
    selectedCameraIndex_ = 0;
    showCameraInfo_ = false;
    showCameraDialog_ = true;
}

void UIManager::showCameraDetectionDialogWithInfo(const std::vector<CameraInfo>& cameraInfos) {
    detectedCameraInfos_ = cameraInfos;
    detectedCameras_.clear();
    for (const auto& info : cameraInfos) {
        detectedCameras_.push_back(info.name);
    }
    selectedCameraIndex_ = 0;
    showCameraInfo_ = true;
    showCameraDialog_ = true;
}

void UIManager::setCameraSelectedCallback(std::function<void(int)> callback) {
    cameraSelectedCallback_ = callback;
}

void UIManager::setDeviceInfo(const std::string& deviceInfo) {
    deviceInfo_ = deviceInfo;
}

void UIManager::defaultPowerCallback() {
    std::cout << "Power button clicked - Exiting application" << std::endl;
    isRunning_ = false;
}

void UIManager::defaultMenuCallback() {
    std::cout << "Menu button clicked - Showing About dialog" << std::endl;
    showAboutDialog();
}

void UIManager::defaultRangeCallback() {
    std::cout << "Range button clicked" << std::endl;
}

void UIManager::defaultZoomCallback() {
    std::cout << "Zoom button clicked" << std::endl;
}

void UIManager::defaultCaptureCallback() {
    std::cout << "Capture button clicked" << std::endl;
}

void UIManager::defaultPseudoColorCallback() {
    std::cout << "Pseudo Color button clicked" << std::endl;
}
