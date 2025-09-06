@echo off
chcp 65001 >nul
echo ========================================
echo    设计模式示例编译脚本
echo ========================================
echo.

REM 检查CMake是否安装
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ❌ 错误：未找到CMake，请先安装CMake
    echo    下载地址：https://cmake.org/download/
    pause
    exit /b 1
) else (
    echo ✅ 找到CMake
)

REM 检查build目录是否存在，不存在则创建
if not exist build (
    echo 📁 创建build目录...
    mkdir build
    echo ✅ build目录创建成功
) else (
    echo 📁 build目录已存在
)

echo.
echo 🔧 配置项目...
cd build
cmake ..
if errorlevel 1 (
    echo ❌ CMake配置失败
    cd ..
    pause
    exit /b 1
) else (
    echo ✅ CMake配置成功
)

echo.
echo 🏗️  编译项目...
cmake --build . --config Release
if errorlevel 1 (
    echo ❌ 编译失败
    cd ..
    pause
    exit /b 1
) else (
    echo ✅ 编译成功
)

echo.
echo ========================================
echo    编译完成！
echo ========================================
echo.

REM 统计生成的可执行文件数量
set /a exe_count=0
for %%f in (Release\*.exe) do set /a exe_count+=1

echo 📊 生成的可执行文件：
echo    位置：%~dp0build\Release\
echo    数量：%exe_count%个
echo.

echo 📋 可执行文件列表：
for %%f in (Release\*.exe) do (
    echo    - %%~nf.exe
)

echo.
echo 💡 使用说明：
echo    运行示例：.\build\Release\singleton_pattern.exe
echo    查看帮助：.\build\Release\pattern_name.exe --help (如果支持)
echo.

cd ..
echo 按任意键退出...
pause >nul
