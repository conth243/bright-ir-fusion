#include <opencv2/opencv.hpp>
#include <iostream>
#include "ui_manager.h"

int main() {
    std::cout << "=== Bright IR Fusion - UI Demo ===" << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    
    // Create UI Manager
    UIManager ui;
    
    // Initialize UI (1024x768 window, center 640x512 image display area)
    if (!ui.initialize("Bright IR Fusion", 1024, 768)) {
        std::cout << "Failed to initialize UI" << std::endl;
        return -1;
    }
    
    std::cout << "UI initialized successfully" << std::endl;
    std::cout << "Window size: 1024x768" << std::endl;
    std::cout << "Display area: 640x512 (center)" << std::endl;
    std::cout << "Left buttons: Power, Menu, Range" << std::endl;
    std::cout << "Right buttons: Zoom, Capture, Pseudo" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
    
    // Create a sample image
    cv::Mat sampleImage(512, 640, CV_8UC3, cv::Scalar(40, 40, 40));
    cv::putText(sampleImage, "Image Display Area", cv::Point(200, 256),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(200, 200, 200), 2);
    cv::putText(sampleImage, "640 x 512", cv::Point(250, 300),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(150, 150, 150), 2);
    
    ui.setDisplayImage(sampleImage);
    
    // Main loop
    while (ui.isRunning()) {
        // Render UI
        ui.render();
        
        // Handle keyboard events
        int key = cv::waitKey(30);
        ui.handleKeyEvent(key);
    }
    
    // Close UI
    ui.close();
    
    std::cout << "Application closed successfully" << std::endl;
    
    return 0;
}