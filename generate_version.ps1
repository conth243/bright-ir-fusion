# 生成版本号的PowerShell脚本

# 配置文件路径
$configFile = "version.ini"

# 基础版本号
$baseVersion = "v1.1"

# 获取当前日期和时间
$now = Get-Date
$currentDate = $now.ToString("yyyyMMdd")
$currentDateTime = $now.ToString("yyyyMMddHHmm")

# 读取配置文件
$buildDate = ""
$serialNumber = 1

if (Test-Path $configFile) {
    $lines = Get-Content $configFile
    foreach ($line in $lines) {
        if ($line -match "^build_date=(.+)$") {
            $buildDate = $matches[1]
        } elseif ($line -match "^serial_number=(.+)$") {
            $serialNumber = [int]$matches[1]
        }
    }
}

# 检查是否是新的一天，如果是，重置序列号
if ($buildDate -ne $currentDate) {
    $buildDate = $currentDate
    $serialNumber = 1
}

# 生成版本号
$version = $baseVersion + "_" + $currentDateTime + "_" + ("{0:000}" -f $serialNumber)

# 写回配置文件
$content = @(
    "version=$version",
    "base_version=$baseVersion",
    "build_date=$buildDate",
    "serial_number=$($serialNumber + 1)"
)
$content | Out-File -FilePath $configFile -Encoding UTF8 -Force

# 输出版本号
Write-Output "Generated version: $version"
