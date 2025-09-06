@echo off
chcp 65001 >nul
echo ========================================
echo    设计模式结构图PDF生成脚本
echo ========================================
echo.

echo 开始生成PDF文件...
echo.

REM 遍历所有.dot文件并转换为PDF
for %%f in (*.dot) do (
    echo 正在转换: %%~nf.dot -> %%~nf.pdf
    dot -Tpdf "%%f" -o "%%~nf.pdf"
    if errorlevel 1 (
        echo ❌ 转换失败: %%~nf.dot
    ) else (
        echo ✅ 转换成功: %%~nf.pdf
    )
    echo.
)

echo ========================================
echo    PDF生成完成！
echo ========================================
echo.

REM 统计生成的PDF文件数量
set /a count=0
for %%f in (*.pdf) do set /a count+=1

echo 生成的PDF文件数量: %count%
echo.

echo 按任意键退出...
pause >nul
