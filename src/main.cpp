#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "=== Iteration 1: Core Module and Basic Framework ===" << std::endl;
    
    // Check OpenCV version
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    
    // Create a simple image
    cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255));
    
    // Add text to the image
    cv::putText(image, "Core Module Test", cv::Point(50, 100), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    cv::putText(image, "OpenCV Version: " + std::string(CV_VERSION), 
                cv::Point(50, 150), cv::FONT_HERSHEY_SIMPLEX, 1, 
                cv::Scalar(0, 0, 255), 2);
    cv::putText(image, "Iteration 1 Validation", 
                cv::Point(50, 200), cv::FONT_HERSHEY_SIMPLEX, 1, 
                cv::Scalar(255, 0, 0), 2);
    cv::putText(image, "Press ESC to exit", cv::Point(50, 250), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    
    // Create display window
    cv::namedWindow("Core Module Test", cv::WINDOW_NORMAL);
    
    // Display image
    cv::imshow("Core Module Test", image);
    
    std::cout << "Core module test window created" << std::endl;
    std::cout << "Press ESC to exit test" << std::endl;
    
    // Wait for key press
    while (true) {
        int key = cv::waitKey(1);
        if (key == 27) { // ESC key
            break;
        }
    }
    
    // Release resources
    cv::destroyWindow("Core Module Test");
    
    std::cout << "Core module test completed" << std::endl;
    std::cout << "Iteration 1 validation successful!" << std::endl;
    
    return 0;
}