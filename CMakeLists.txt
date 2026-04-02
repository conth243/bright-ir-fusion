cmake_minimum_required(VERSION 3.10)

project(bif)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置OpenCV目录
set(OpenCV_DIR "C:/opencv/build/x64/vc18/lib")

# 查找OpenCV
find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
    message("OpenCV found: ${OpenCV_VERSION}")
else()
    message(FATAL_ERROR "OpenCV not found")
endif()

# 包含目录
include_directories(include)

# 源文件
set(SOURCE_FILES
    src/main.cpp
    src/camera.cpp
    src/ui_manager.cpp
)

# 生成版本号
set(BASE_VERSION "v1.1")
set(CONFIG_FILE "version.ini")

# 获取当前日期和时间
string(TIMESTAMP CURRENT_DATE "%Y%m%d" UTC)
string(TIMESTAMP CURRENT_DATETIME "%Y%m%d%H%M" UTC)

# 读取配置文件
if(EXISTS ${CONFIG_FILE})
    file(READ ${CONFIG_FILE} CONFIG_CONTENT)
    string(REGEX MATCH "build_date=(.*)" BUILD_DATE_MATCH ${CONFIG_CONTENT})
    if(BUILD_DATE_MATCH)
        string(REPLACE "build_date=" "" BUILD_DATE ${BUILD_DATE_MATCH})
    endif()
    string(REGEX MATCH "serial_number=(.*)" SERIAL_NUMBER_MATCH ${CONFIG_CONTENT})
    if(SERIAL_NUMBER_MATCH)
        string(REPLACE "serial_number=" "" SERIAL_NUMBER ${SERIAL_NUMBER_MATCH})
    else()
        set(SERIAL_NUMBER 1)
    endif()
else()
    set(BUILD_DATE "")
    set(SERIAL_NUMBER 1)
endif()

# 检查是否是新的一天，如果是，重置序列号
if(NOT BUILD_DATE STREQUAL CURRENT_DATE)
    set(BUILD_DATE ${CURRENT_DATE})
    set(SERIAL_NUMBER 1)
endif()

# 生成版本号
string(TOUPPER ${SERIAL_NUMBER} SERIAL_NUMBER_UPPER)
string(LENGTH ${SERIAL_NUMBER} SERIAL_NUMBER_LENGTH)
if(SERIAL_NUMBER_LENGTH EQUAL 1)
    set(SERIAL_NUMBER_PADDED "00${SERIAL_NUMBER}")
elseif(SERIAL_NUMBER_LENGTH EQUAL 2)
    set(SERIAL_NUMBER_PADDED "0${SERIAL_NUMBER}")
else()
    set(SERIAL_NUMBER_PADDED ${SERIAL_NUMBER})
endif()
set(VERSION "${BASE_VERSION}_${CURRENT_DATETIME}_${SERIAL_NUMBER_PADDED}")

# 计算下一个序列号
math(EXPR NEXT_SERIAL_NUMBER "${SERIAL_NUMBER} + 1")

# 写回配置文件
file(WRITE ${CONFIG_FILE} "version=${VERSION}\n")
file(APPEND ${CONFIG_FILE} "base_version=${BASE_VERSION}\n")
file(APPEND ${CONFIG_FILE} "build_date=${BUILD_DATE}\n")
file(APPEND ${CONFIG_FILE} "serial_number=${NEXT_SERIAL_NUMBER}\n")

# 输出版本号
message("Generated version: ${VERSION}")

# 目标可执行文件
add_executable(bif ${SOURCE_FILES})

# 链接库
target_link_libraries(bif ${OpenCV_LIBS} strmiids.lib ole32.lib oleaut32.lib uuid.lib)

# 确保输出目录存在
add_custom_command(
    TARGET bif
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:bif>
    COMMENT "Creating output directory"
)

# 复制版本文件到输出目录
add_custom_command(
    TARGET bif
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/${CONFIG_FILE}
    $<TARGET_FILE_DIR:bif>
    COMMENT "Copying version.ini to output directory"
)
