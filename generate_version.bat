@echo off

REM 生成版本号的批处理脚本

REM 配置文件路径
set configFile=version.ini

REM 基础版本号
set baseVersion=v1.1

REM 获取当前日期和时间
for /f "tokens=2-8 delims=/:. " %%a in ('echo %date% %time%') do (set year=%%c& set month=%%a& set day=%%b& set hour=%%d& set minute=%%e)

REM 确保月份和日期是两位数
if %month% lss 10 set month=0%month%
if %day% lss 10 set day=0%day%
if %hour% lss 10 set hour=0%hour%
if %minute% lss 10 set minute=0%minute%

set currentDate=%year%%month%%day%
set currentDateTime=%year%%month%%day%%hour%%minute%

REM 读取配置文件
set buildDate=
set serialNumber=1

if exist %configFile% (
    for /f "tokens=1,2 delims==" %%a in (%configFile%) do (
        if "%%a"=="build_date" set buildDate=%%b
        if "%%a"=="serial_number" set serialNumber=%%b
    )
)

REM 检查是否是新的一天，如果是，重置序列号
if not "%buildDate%"=="%currentDate%" (
    set buildDate=%currentDate%
    set serialNumber=1
)

REM 生成版本号
set version=%baseVersion%_%currentDateTime%_%serialNumber%

REM 确保序列号是三位数
if %serialNumber% lss 10 set version=%baseVersion%_%currentDateTime%_00%serialNumber%
if %serialNumber% geq 10 if %serialNumber% lss 100 set version=%baseVersion%_%currentDateTime%_0%serialNumber%

REM 计算下一个序列号
set /a nextSerialNumber=%serialNumber%+1

REM 写回配置文件
echo version=%version% > %configFile%
echo base_version=%baseVersion% >> %configFile%
echo build_date=%buildDate% >> %configFile%
echo serial_number=%nextSerialNumber% >> %configFile%

REM 输出版本号
echo Generated version: %version%
