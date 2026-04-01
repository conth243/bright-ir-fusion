#include <opencv2/opencv.hpp>
#include <iostream>
#include "ui_manager.h"
#include "camera.h"

int main() {
    std::cout << "=== Bright IR Fusion - UVC Camera Demo ===" << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    
    UIManager ui;
    
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
    
    std::vector<CameraInfo> cameraInfos = enumerateCamerasWithParams();
    std::cout << "Available cameras:" << std::endl;
    for (size_t i = 0; i < cameraInfos.size(); i++) {
        const auto& info = cameraInfos[i];
        std::cout << "  " << i << ": " << info.name << " (" << info.width << "x" << info.height << ", " << info.fps << "fps)" << std::endl;
    }
    
    Camera camera;
    bool cameraInitialized = false;
    int selectedCameraIndex = -1;
    
    if (cameraInfos.empty()) {
        ui.setDeviceInfo("No camera found");
    } else {
        ui.setDeviceInfo("Please select a camera");
    }
    
    ui.setCameraSelectedCallback([&](int index) {
        selectedCameraIndex = index;
        const auto& info = cameraInfos[index];
        std::cout << "Selected camera: " << info.name << std::endl;
        
        if (cameraInitialized) {
            camera.release();
        }
        
        if (camera.initialize(index, 640, 480, 30)) {
            std::cout << "Camera initialized successfully" << std::endl;
            std::cout << "Camera name: " << camera.getDeviceName() << std::endl;
            std::cout << "Resolution: " << camera.getWidth() << "x" << camera.getHeight() << std::endl;
            std::cout << "FPS: " << camera.getFPS() << std::endl;
            cameraInitialized = true;
            
            std::string deviceInfo = "Camera Name: " + camera.getDeviceName() + "\n" +
                                     "Resolution: " + std::to_string(camera.getWidth()) + "x" + std::to_string(camera.getHeight()) + "\n" +
                                     "Frame Rate: " + std::to_string(camera.getFPS()) + "fps";
            ui.setDeviceInfo(deviceInfo);
        } else {
            std::cout << "Failed to initialize camera" << std::endl;
            cameraInitialized = false;
            
            ui.setDeviceInfo("Failed to initialize camera");
        }
    });
    
    ui.showCameraDetectionDialogWithInfo(cameraInfos);
    
    cv::Mat frame;
    while (ui.isRunning()) {
        if (cameraInitialized && camera.capture(frame)) {
            cv::Mat resizedFrame;
            cv::resize(frame, resizedFrame, cv::Size(640, 512));
            ui.setDisplayImage(resizedFrame);
        }
        
        ui.render();
        
        int key = cv::waitKey(30);
        ui.handleKeyEvent(key);
    }
    
    if (cameraInitialized) {
        camera.release();
    }
    ui.close();
    
    std::cout << "Application closed successfully" << std::endl;
    
    return 0;
}