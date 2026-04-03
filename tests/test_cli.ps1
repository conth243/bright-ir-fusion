# 测试CLI系统的PowerShell脚本

Write-Host "=== Testing Bright IR Fusion CLI System ===" -ForegroundColor Green

# 检查bif.exe是否存在
if (-not (Test-Path "./bif.exe")) {
    Write-Host "✗ bif.exe not found" -ForegroundColor Red
    exit 1
}

Write-Host "✓ bif.exe found" -ForegroundColor Green

# 创建进程对象
$process = New-Object System.Diagnostics.Process
$process.StartInfo.FileName = "./bif.exe"
$process.StartInfo.UseShellExecute = $false
$process.StartInfo.RedirectStandardInput = $true
$process.StartInfo.RedirectStandardOutput = $true
$process.StartInfo.CreateNoWindow = $true

# 启动进程
Write-Host "Starting bif.exe..." -ForegroundColor Yellow
$process.Start() | Out-Null

# 等待进程启动
Write-Host "Waiting for process to start..." -ForegroundColor Yellow
Start-Sleep -Seconds 3

# 发送命令
Write-Host "Sending commands..." -ForegroundColor Yellow
$process.StandardInput.WriteLine("help")
$process.StandardInput.WriteLine("version")
$process.StandardInput.WriteLine("status")
$process.StandardInput.WriteLine("list-cameras")
$process.StandardInput.WriteLine("select-camera 0")
$process.StandardInput.WriteLine("status")
$process.StandardInput.WriteLine("exit")
$process.StandardInput.Close()

# 读取输出
Write-Host "Reading output..." -ForegroundColor Yellow
$output = $process.StandardOutput.ReadToEnd()

# 等待进程结束
Write-Host "Waiting for process to exit..." -ForegroundColor Yellow
$process.WaitForExit()

# 显示输出
Write-Host "=== CLI Output ===" -ForegroundColor Cyan
Write-Host $output

# 检查关键命令的输出
Write-Host "=== Testing Results ===" -ForegroundColor Yellow

if ($output -match "Available commands:") {
    Write-Host "✓ help command works" -ForegroundColor Green
} else {
    Write-Host "✗ help command failed" -ForegroundColor Red
}

if ($output -match "Version:") {
    Write-Host "✓ version command works" -ForegroundColor Green
} else {
    Write-Host "✗ version command failed" -ForegroundColor Red
}

if ($output -match "Current status:") {
    Write-Host "✓ status command works" -ForegroundColor Green
} else {
    Write-Host "✗ status command failed" -ForegroundColor Red
}

if ($output -match "Available cameras:") {
    Write-Host "✓ list-cameras command works" -ForegroundColor Green
} else {
    Write-Host "✗ list-cameras command failed" -ForegroundColor Red
}

if ($output -match "Selected camera:") {
    Write-Host "✓ select-camera command works" -ForegroundColor Green
} else {
    Write-Host "✗ select-camera command failed" -ForegroundColor Red
}

if ($output -match "Exiting CLI...") {
    Write-Host "✓ exit command works" -ForegroundColor Green
} else {
    Write-Host "✗ exit command failed" -ForegroundColor Red
}

Write-Host "=== Test Complete ===" -ForegroundColor Green
