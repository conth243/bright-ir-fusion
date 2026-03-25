cmake_minimum_required(VERSION 3.10)
project(StereoCameraApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置包含目录
include_directories(include)

# 添加MSVC和Windows SDK的包含路径
include_directories("D:/VS/BuildTools/VC/Tools/MSVC/14.44.35207/include")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared")
include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um")

# 设置OpenCV路径
set(OpenCV_DIR "C:/opencv/build")

# 查找OpenCV库
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
else()
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV and set OpenCV_DIR environment variable.")
endif()

# 添加可执行文件
add_executable(StereoCameraApp src/main.cpp)

# 链接库
target_link_libraries(StereoCameraApp ${OpenCV_LIBS} gdi32 user32)

# 添加MSVC和Windows SDK的库路径
link_directories("D:/VS/BuildTools/VC/Tools/MSVC/14.44.35207/lib/x64")
link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/ucrt/x64")
link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64")
