# 测试CLI系统的简单PowerShell脚本

Write-Host "=== Testing Bright IR Fusion CLI System ===" -ForegroundColor Green

# 检查bif.exe是否存在
if (-not (Test-Path "./bif.exe")) {
    Write-Host "✗ bif.exe not found" -ForegroundColor Red
    exit 1
}

Write-Host "✓ bif.exe found" -ForegroundColor Green

# 运行bif.exe并捕获输出
Write-Host "Running bif.exe..." -ForegroundColor Yellow
$output = & "./bif.exe" 2>&1

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
