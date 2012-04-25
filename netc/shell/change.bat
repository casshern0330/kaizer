@echo off
CLS
color 17
@title 修改本机IP地址 By hucw 2012-04-10

:title
echo.	==<<修改本机IP地址 >>==

echo.	1 切换到局方外网

echo.	2 切换到局方内网

echo.	3 切换到分支网络

echo.	9 退出

echo.	请输入选择项目的序号并按回车键

set /p XUANXIANG=

if "%XUANXIANG%"=="1" goto 1
if "%XUANXIANG%"=="2" goto 2
if "%XUANXIANG%"=="3" goto 3
if "%XUANXIANG%"=="9" goto 9

:1
echo 切换到局方外网

netsh interface ip set address name = "本地连接" source=dhcp
netsh interface ip set dns name="本地连接" source=dhcp

echo 成功切换到局方外网
pause
CLS
goto title

:2
echo 切换到局方内网

nterface ip set address "本地连接" static 10.208.231.56 255.255.255.0 10.208.231.254 1

echo 成功切换到局方内网
pause
CLS
goto title

:9
