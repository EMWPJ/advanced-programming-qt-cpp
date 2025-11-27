@echo off
chcp 65001 >nul
echo ===== 编译 Chapter 07 代码 =====
echo.

echo 清理旧的构建目录和缓存...
if exist build (
    echo 删除 build 目录...
    rmdir /s /q build
)
if exist CMakeCache.txt del /q CMakeCache.txt 2>nul
if exist CMakeFiles rmdir /s /q CMakeFiles 2>nul
if exist *.cmake del /q *.cmake 2>nul

echo.
echo 创建构建目录...
mkdir build 2>nul
cd build

echo.
echo 运行 CMake 配置...

cmake .. -DCMAKE_BUILD_TYPE=Release 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo.
    cmake .. -DCMAKE_BUILD_TYPE=Release 2>&1
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ===== CMake 配置失败！ =====
    echo 请检查错误信息
    echo.
    echo 提示：如果路径包含中文字符，可能需要：
    echo 1. 将项目移动到英文路径
    echo 2. 或者手动运行: cmake .. -G "Unix Makefiles"
    cd ..
    pause
    exit /b 1
)

echo.
echo CMake 配置成功！
echo.
echo 开始编译...
echo 注意：如果遇到 LNK1104 错误，可能是并行编译冲突，将使用单线程编译...
cmake --build . --config Release -j 2
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo 并行编译失败，尝试单线程编译...
    cmake --build . --config Release -j 1
)

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===== 编译完成！ =====
    echo 可执行文件位于 build 目录中
) else (
    echo.
    echo ===== 编译过程中出现错误 =====
    echo 请检查上面的错误信息
    echo.
)

cd ..
pause
