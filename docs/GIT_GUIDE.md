# Git 仓库使用指南

## 仓库位置

本项目的 Git 仓库位于：`D:\ai\Trae\bright-ir-fusion`

当前项目目录 `D:\ai\Trae\BIF` 是项目代码，但不是 Git 仓库目录。所有 git 操作都需要在 `D:\ai\Trae\bright-ir-fusion` 目录下进行。

## 初始化和配置

1. **仓库已初始化**：当前目录已经是一个初始化好的 Git 仓库
2. **第一次使用**：
   - 配置用户名：`git config user.name "Your Name"`
   - 配置邮箱：`git config user.email "your.email@example.com"`

## 基本操作

### 添加文件
```bash
git add .
```

### 提交更改
```bash
git commit -m "提交消息"
```

### 查看状态
```bash
git status
```

### 查看提交历史
```bash
git log
```

## 分支管理

### 创建分支
```bash
git checkout -b feature-branch
```

### 切换分支
```bash
git checkout master
```

## 注意事项

1. **不要在其他目录初始化新的 Git 仓库**，本项目的所有版本控制操作都应该在 `D:\ai\Trae\BIF` 目录下进行
2. **定期提交**：对代码的修改应该及时提交，确保代码安全
3. **编写有意义的提交消息**：清晰描述每次提交的内容和目的
4. **分支管理**：使用分支进行功能开发，避免直接在 master 分支上进行开发

## 编译和运行

### 编译项目
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 运行程序
```bash
./bif.exe
```
