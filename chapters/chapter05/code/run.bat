@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul
echo ========================================
echo    设计模式示例运行脚本
echo ========================================
echo.

REM 检查build\Release目录是否存在
if not exist build\Release (
    echo ❌ 错误：未找到编译好的可执行文件
    echo    请先运行 build.bat 进行编译
    echo.
    pause
    exit /b 1
)

echo 📋 可用的设计模式示例：
echo.

REM 列出所有可执行文件
set /a count=0
for %%f in (build\Release\*.exe) do (
    set /a count+=1
    echo !count!. %%~nf
)

if %count%==0 (
    echo ❌ 未找到任何可执行文件
    pause
    exit /b 1
)

echo.
echo ========================================
echo.

REM 如果只有一个参数，则直接运行
if "%1"=="" goto :menu
goto :run_direct

:menu
echo 请输入要运行的设计模式编号 (1-%count%)：
echo 输入 'q' 退出
echo.
set /p choice="选择: "

if "%choice%"=="q" goto :exit
if "%choice%"=="Q" goto :exit

REM 验证输入是否为数字
echo %choice%| findstr /r "^[0-9]*$" >nul
if errorlevel 1 (
    echo ❌ 请输入有效的数字
    goto :menu
)

if %choice% lss 1 (
    echo ❌ 编号不能小于1
    goto :menu
)
if %choice% gtr %count% (
    echo ❌ 编号不能大于%count%
    goto :menu
)

goto :run_by_number

:run_direct
REM 直接运行指定的模式
set pattern=%1
if exist "build\Release\%pattern%.exe" (
    echo 🚀 运行 %pattern%...
    echo.
    "build\Release\%pattern%.exe"
) else (
    echo ❌ 未找到 %pattern%.exe
    echo.
    echo 可用模式：
    for %%f in (build\Release\*.exe) do (
        echo   - %%~nf
    )
)
goto :exit

:run_by_number
REM 根据编号运行
set /a index=0
for %%f in (build\Release\*.exe) do (
    set /a index+=1
    if !index! equ %choice% (
        echo 🚀 运行 %%~nf...
        echo.
        "%%f"
        goto :exit
    )
)

:exit
echo.
echo ========================================
echo    运行完成！
echo ========================================
echo.
echo 按任意键退出...
pause >nul
