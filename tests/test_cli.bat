@echo off

echo === Testing Bright IR Fusion CLI System ===
echo.

:: 检查bif.exe是否存在
if not exist "bif.exe" (
    echo ✗ bif.exe not found
    pause
    exit /b 1
)

echo ✓ bif.exe found
echo.

:: 创建测试输入文件
echo help > test_input.txt
echo version >> test_input.txt
echo status >> test_input.txt
echo list-cameras >> test_input.txt
echo select-camera 0 >> test_input.txt
echo status >> test_input.txt
echo exit >> test_input.txt

echo Starting bif.exe...
echo Sending commands...
echo.

:: 运行bif.exe并将输入和输出重定向
bif.exe < test_input.txt > test_output.txt 2>&1

echo Reading output...
echo.

:: 显示输出
echo === CLI Output ===
type test_output.txt
echo.

:: 检查关键命令的输出
echo === Testing Results ===

type test_output.txt | find "Available commands:" > nul
if %errorlevel% equ 0 (
    echo ✓ help command works
) else (
    echo ✗ help command failed
)

type test_output.txt | find "Version:" > nul
if %errorlevel% equ 0 (
    echo ✓ version command works
) else (
    echo ✗ version command failed
)

type test_output.txt | find "Current status:" > nul
if %errorlevel% equ 0 (
    echo ✓ status command works
) else (
    echo ✗ status command failed
)

type test_output.txt | find "Available cameras:" > nul
if %errorlevel% equ 0 (
    echo ✓ list-cameras command works
) else (
    echo ✗ list-cameras command failed
)

type test_output.txt | find "Selected camera:" > nul
if %errorlevel% equ 0 (
    echo ✓ select-camera command works
) else (
    echo ✗ select-camera command failed
)

type test_output.txt | find "Exiting CLI..." > nul
if %errorlevel% equ 0 (
    echo ✓ exit command works
) else (
    echo ✗ exit command failed
)

echo.
echo === Test Complete ===

:: 清理临时文件
del test_input.txt test_output.txt /q

pause
