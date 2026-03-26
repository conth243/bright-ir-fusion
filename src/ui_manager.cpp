#include "ui_manager.h"
#include <opencv2/opencv.hpp>
#include <iostream>

UIManager::UIManager() 
    : windowWidth_(1024)
    , windowHeight_(768)
    , isRunning_(false)
    , showAbout_(false)
    , versionInfo_("Bright IR Fusion v1.0\nOpenCV Version: " + std::string(CV_VERSION)) {
}

UIManager::~UIManager() {
    close();
}

bool UIManager::initialize(const std::string& windowName, int windowWidth, int windowHeight) {
    windowName_ = windowName;
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
    
    // Create window
    cv::namedWindow(windowName_, cv::WINDOW_NORMAL);
    cv::resizeWindow(windowName_, windowWidth_, windowHeight_);
    
    // Set display area (center 640x512)
    int displayX = (windowWidth_ - 640) / 2;
    int displayY = (windowHeight_ - 512) / 2;
    displayRect_ = cv::Rect(displayX, displayY, 640, 512);
    
    // Create background
    background_ = cv::Mat(windowHeight_, windowWidth_, CV_8UC3, cv::Scalar(50, 50, 50));
    
    // Initialize buttons
    initializeButtons();
    
    // Set mouse callback
    cv::setMouseCallback(windowName_, [](int event, int x, int y, int flags, void* userdata) {
        UIManager* ui = static_cast<UIManager*>(userdata);
        ui->handleMouseEvent(event, x, y, flags);
    }, this);
    
    isRunning_ = true;
    return true;
}

void UIManager::initializeButtons() {
    // Button size and spacing
    int buttonWidth = 80;
    int buttonHeight = 40;
    int buttonSpacing = 20;
    int leftButtonX = 50;
    int rightButtonX = windowWidth_ - 50 - buttonWidth;
    int startY = (windowHeight_ - (3 * buttonHeight + 2 * buttonSpacing)) / 2;
    
    // Left buttons (top to bottom: Power, Menu, Range)
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
    
    // Right buttons (top to bottom: Zoom, Capture, Pseudo)
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
    
    // Draw display area border
    cv::rectangle(canvas, displayRect_, cv::Scalar(200, 200, 200), 2);
    
    // Draw image
    if (!displayImage_.empty()) {
        displayImage_.copyTo(canvas(displayRect_));
    } else {
        cv::Mat placeholder(displayRect_.height, displayRect_.width, CV_8UC3, cv::Scalar(30, 30, 30));
        cv::putText(placeholder, "No Image", cv::Point(displayRect_.width/2 - 50, displayRect_.height/2),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(150, 150, 150), 2);
        placeholder.copyTo(canvas(displayRect_));
    }
    
    // Draw left buttons
    for (auto& button : leftButtons_) {
        drawButton(canvas, button);
    }
    
    // Draw right buttons
    for (auto& button : rightButtons_) {
        drawButton(canvas, button);
    }
    
    // Draw about dialog
    if (showAbout_) {
        drawAboutDialog(canvas);
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
    int dialogWidth = 400;
    int dialogHeight = 300;
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
    
    cv::putText(canvas, "Press ESC or click outside to close", cv::Point(dialogX + 20, dialogY + dialogHeight - 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(150, 150, 150), 1);
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

void UIManager::defaultPowerCallback() {
    std::cout << "Power button clicked" << std::endl;
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