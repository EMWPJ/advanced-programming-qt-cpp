@echo off
setlocal enabledelayedexpansion

echo ====================================
echo    LaTeX文档重建脚本
echo ====================================

REM 设置变量
set "LATEX_CMD=xelatex -shell-escape -interaction=nonstopmode"
set "CLEANUP=0"

REM 检查命令行参数
if "%1"=="clean" (
    set "CLEANUP=1"
    goto cleanup
)
if "%1"=="help" goto help

REM 编译所有章节
for /L %%i in (1,1,6) do (
    call :compile_chapter %%i
)

echo.
echo ====================================
echo    编译完成！
echo ====================================
goto end

:compile_chapter
set "CHAPTER_NUM=%1"
set "CHAPTER_DIR=chapter%CHAPTER_NUM%"
set "CHAPTER_FILE=%CHAPTER_DIR%.tex"

echo.
echo 正在编译 %CHAPTER_DIR%...

if not exist "%CHAPTER_DIR%" (
    echo 警告: 目录 %CHAPTER_DIR% 不存在，跳过
    goto :eof
)

cd "%CHAPTER_DIR%"

REM 清理旧的辅助文件
if exist "*.aux" del "*.aux"
if exist "*.log" del "*.log"
if exist "*.out" del "*.out"
if exist "*.toc" del "*.toc"
if exist "*.nav" del "*.nav"
if exist "*.snm" del "*.snm"
if exist "*.vrb" del "*.vrb"
if exist "_minted-*" rmdir /s /q "_minted-*"

REM 第一次编译
echo 第一次编译 %CHAPTER_FILE%...
%LATEX_CMD% "%CHAPTER_FILE%"

REM 检查编译是否成功
if errorlevel 1 (
    echo 错误: 第一次编译失败
    cd ..
    goto :eof
)

REM 第二次编译（处理交叉引用）
echo 第二次编译 %CHAPTER_FILE%...
%LATEX_CMD% "%CHAPTER_FILE%"

if errorlevel 1 (
    echo 错误: 第二次编译失败
    cd ..
    goto :eof
)

echo %CHAPTER_DIR% 编译成功！
cd ..
goto :eof

:cleanup
echo 清理所有章节的辅助文件...
for /L %%i in (1,1,6) do (
    set "CHAPTER_DIR=chapter%%i"
    if exist "!CHAPTER_DIR!" (
        cd "!CHAPTER_DIR!"
        echo 清理 !CHAPTER_DIR!...
        if exist "*.aux" del "*.aux"
        if exist "*.log" del "*.log"
        if exist "*.out" del "*.out"
        if exist "*.toc" del "*.toc"
        if exist "*.nav" del "*.nav"
        if exist "*.snm" del "*.snm"
        if exist "*.vrb" del "*.vrb"
        if exist "_minted-*" rmdir /s /q "_minted-*"
        cd ..
    )
)
echo 清理完成！
goto end

:help
echo.
echo 使用方法:
echo   rebuild.bat          编译所有章节
echo   rebuild.bat clean    清理所有辅助文件
echo   rebuild.bat help     显示此帮助信息
echo.
echo 功能特点:
echo   - 自动清理旧的辅助文件
echo   - 两次编译确保交叉引用正确
echo   - 错误检查和报告
echo   - 支持chapter01到chapter06
goto end

:end
echo.
pause
