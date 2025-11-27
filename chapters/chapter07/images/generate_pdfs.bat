@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul
echo ========================================
echo    科学计算库结构图PDF生成脚本
echo    递归遍历子文件夹生成PDF
echo ========================================
echo.

echo 开始生成PDF文件...
echo.

REM 设置工作目录为脚本所在目录（images 文件夹）
set "WORK_DIR=%~dp0"
cd /d "%WORK_DIR%"

REM 统计变量
set /a total_count=0
set /a success_count=0
set /a fail_count=0

REM 递归遍历所有子文件夹，优先生成总图
echo [总图] 生成总览图...
for /r %%d in (.) do (
    pushd "%%d"
    for %%f in (*_overview.dot) do (
        set /a total_count+=1
        echo 正在转换: %%~dpf%%~nf.dot -^> %%~dpf%%~nf.pdf
        dot -Tpdf "%%f" -o "%%~nf.pdf"
        if errorlevel 1 (
            echo ❌ 转换失败: %%~dpf%%~nf.dot
            set /a fail_count+=1
        ) else (
            echo ✅ 转换成功: %%~dpf%%~nf.pdf
            set /a success_count+=1
        )
        echo.
    )
    popd
)

REM 递归遍历所有子文件夹，生成分图
echo [分图] 生成详细分图...
for /r %%d in (.) do (
    pushd "%%d"
    REM 检查是否在 scripts 文件夹中，如果是则跳过
    echo "%%d" | findstr /i "\\scripts\\" >nul
    if errorlevel 1 (
        for %%f in (*.dot) do (
            REM 跳过总览图（已在前面处理）
            if not "%%~nf"=="eigen_overview" (
                if not "%%~nf"=="boost_overview" (
                    if not "%%~nf"=="mkl_overview" (
                        set /a total_count+=1
                        echo 正在转换: %%~dpf%%~nf.dot -^> %%~dpf%%~nf.pdf
                        dot -Tpdf "%%f" -o "%%~nf.pdf"
                        if errorlevel 1 (
                            echo ❌ 转换失败: %%~dpf%%~nf.dot
                            set /a fail_count+=1
                        ) else (
                            echo ✅ 转换成功: %%~dpf%%~nf.pdf
                            set /a success_count+=1
                        )
                        echo.
                    )
                )
            )
        )
    )
    popd
)

echo ========================================
echo    PDF生成完成！
echo ========================================
echo.

REM 统计生成的PDF文件数量
set /a pdf_count=0
for /r %%f in (*.pdf) do (
    REM 排除 scripts 文件夹中的 PDF
    echo "%%f" | findstr /i "\\scripts\\" >nul
    if errorlevel 1 (
        set /a pdf_count+=1
    )
)

echo 统计信息:
echo   处理的DOT文件总数: !total_count!
echo   成功转换: !success_count!
echo   转换失败: !fail_count!
echo   生成的PDF文件总数: !pdf_count!
echo.

echo 按任意键退出...
pause >nul
