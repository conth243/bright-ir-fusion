#include "camera.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <dshow.h>

// 链接DirectShow库
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

// 枚举所有视频捕获设备，返回设备名称列表
std::vector<std::string> enumerateVideoCaptureDevices() {
    std::vector<std::string> deviceNames;
    
    // 初始化COM
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        return deviceNames;
    }
    
    ICreateDevEnum *pDevEnum = nullptr;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pDevEnum);
    if (FAILED(hr)) {
        CoUninitialize();
        return deviceNames;
    }
    
    IEnumMoniker *pEnum = nullptr;
    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
    if (FAILED(hr)) {
        pDevEnum->Release();
        CoUninitialize();
        return deviceNames;
    }
    
    IMoniker *pMoniker = nullptr;
    ULONG cFetched = 0;
    while (pEnum->Next(1, &pMoniker, &cFetched) == S_OK) {
        IPropertyBag *pPropBag = nullptr;
        hr = pMoniker->BindToStorage(nullptr, nullptr, IID_IPropertyBag, (void**)&pPropBag);
        if (SUCCEEDED(hr)) {
            VARIANT var;
            VariantInit(&var);
            hr = pPropBag->Read(L"FriendlyName", &var, nullptr);
            if (SUCCEEDED(hr) && var.vt == VT_BSTR) {
                // 将BSTR转换为std::string
                std::wstring wName(var.bstrVal);
                std::string name(wName.begin(), wName.end());
                deviceNames.push_back(name);
            }
            VariantClear(&var);
            pPropBag->Release();
        }
        pMoniker->Release();
    }
    
    pEnum->Release();
    pDevEnum->Release();
    CoUninitialize();
    
    return deviceNames;
}

Camera::Camera() : deviceIndex(-1), width(0), height(0), fps(0) {
}

Camera::~Camera() {
    release();
}

bool Camera::initialize(int index, int width, int height, int fps) {
    deviceIndex = index;
    this->width = width;
    this->height = height;
    this->fps = fps;
    
    // 打开摄像头
    if (!cap.open(index)) {
        return false;
    }
    
    // 设置摄像头参数
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);
    
    // 获取设备名称
    std::vector<std::string> devices = enumerateVideoCaptureDevices();
    if (!devices.empty() && index < devices.size()) {
        deviceName = devices[index];
    } else {
        deviceName = "Camera " + std::to_string(index);
    }
    
    return true;
}

bool Camera::capture(cv::Mat& frame) {
    if (!cap.isOpened()) {
        return false;
    }
    
    return cap.read(frame);
}

void Camera::release() {
    if (cap.isOpened()) {
        cap.release();
    }
}

std::string Camera::getDeviceName() {
    return deviceName;
}



std::vector<std::string> enumerateCameras() {
    // 使用DirectShow API枚举摄像头设备，获取真实名称
    std::vector<std::string> devices = enumerateVideoCaptureDevices();
    
    // 如果DirectShow枚举失败，回退到原方法
    if (devices.empty()) {
        std::vector<std::string> cameras;
        int index = 0;
        
        while (true) {
            cv::VideoCapture cap(index);
            if (cap.isOpened()) {
                cameras.push_back("Camera " + std::to_string(index));
                cap.release();
                index++;
            } else {
                break;
            }
        }
        
        return cameras;
    }
    
    return devices;
}

bool isCameraAvailable(int index) {
    cv::VideoCapture cap(index);
    bool available = cap.isOpened();
    cap.release();
    return available;
}

std::string getCameraInfo(int index) {
    cv::VideoCapture cap(index);
    if (!cap.isOpened()) {
        return "Camera not available";
    }
    
    std::string info = "Camera " + std::to_string(index);
    
    // 尝试获取摄像头的一些属性
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);
    
    info += " (" + std::to_string(static_cast<int>(width)) + "x" + 
            std::to_string(static_cast<int>(height)) + ", " + 
            std::to_string(static_cast<int>(fps)) + "fps)";
    
    cap.release();
    return info;
}
