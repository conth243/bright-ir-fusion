# 项目规则文件

## 开发环境配置

### 操作系统
- **平台**: Windows 10/11 64-bit

### 编译器工具链
- **编译器**: MSVC 19.50.35728.0 (Visual Studio Build Tools 2026)
- **MSVC路径**: `D:/VS/BuildTools/VC/Tools/MSVC/14.50.35717/`
- **Windows SDK版本**: 10.0.26100.0
- **Windows SDK路径**: `C:/Program Files (x86)/Windows Kits/10/`

### OpenCV配置
- **OpenCV版本**: 4.12.0
- **OpenCV安装路径**: `C:/opencv/`
- **OpenCV预编译库路径**: `C:/opencv/build/x64/vc18/`
  - **重要**: 使用预编译的vc18目录，不要重新编译OpenCV
  - **库文件路径**: `C:/opencv/build/x64/vc18/lib/`
  - **DLL文件路径**: `C:/opencv/build/x64/vc18/bin/`
- **CMake配置**: `set(OpenCV_DIR "C:/opencv/build/x64/vc18/lib")`

### 构建配置
- **构建类型**: 64-bit Release/Debug
- **C++标准**: C++11
- **构建工具**: CMake 4.3.0

## 重要规则

### 1. OpenCV使用规则
- **必须使用预编译库**: 始终使用 `C:/opencv/build/x64/vc18/` 目录下的预编译库
- **禁止重新编译OpenCV**: 不要尝试从源代码编译OpenCV，使用现有的预编译版本
- **CMake配置**: 在CMakeLists.txt中正确设置 `OpenCV_DIR` 变量指向 `C:/opencv/build/x64/vc18/lib`

### 2. 编译器版本规则
- **固定编译器版本**: 使用MSVC 14.50.35717版本，确保兼容性
- **Windows SDK版本**: 使用10.0.26100.0版本，与OpenCV预编译库兼容

### 3. 项目结构规则
- **工作目录**: `D:/ai/Trae/BIF/` 是项目工作目录，用于编译和测试
- **Git仓库**: `D:/ai/Trae/bright-ir-fusion/` 是Git仓库目录，用于版本控制
- **文件同步**: 编译验证通过后，将非中间生成文件同步到Git仓库

### 4. 构建流程
1. 在BIF目录下进行编译和测试
2. 验证通过后，将源代码和最终程序同步到Git仓库
3. 在Git仓库目录进行提交

### 5. 命名规则
- **可执行文件名称**：编译生成的最终目标程序文件必须命名为 `bif.exe`
- **构建输出**：所有构建输出应保持一致性，确保最终交付的可执行文件名为 `bif.exe`

### 6. Git提交规则
- **提交日志语言**：所有git提交的日志信息必须使用中文
- **提交消息格式**：清晰描述本次提交的内容和目的，使用中文撰写
- **提交频率**：对代码的修改应该及时提交，确保代码安全
- **添加文件**：当项目中添加新文件时，需要使用命令 `git add <文件路径>` 将文件添加到Git暂存区，然后再提交
- **删除文件**：当项目中删除文件时，需要使用命令 `git rm <文件路径>` 从Git仓库中删除文件，然后再提交。如果文件已经在本地删除，可以使用命令 `git add -u` 来更新Git暂存区，然后再提交

## 路径速查表

| 组件 | 路径 |
|------|------|
| OpenCV预编译库 | `C:/opencv/build/x64/vc18/` |
| OpenCV CMake配置 | `C:/opencv/build/x64/vc18/lib/` |
| MSVC编译器 | `D:/VS/BuildTools/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/` |
| Windows SDK | `C:/Program Files (x86)/Windows Kits/10/` |
| 项目工作目录 | `D:/ai/Trae/BIF/` |
| Git仓库目录 | `D:/ai/Trae/bright-ir-fusion/` |
