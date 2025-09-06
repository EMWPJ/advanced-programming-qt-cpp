@echo off
chcp 65001 >nul
echo ========================================
echo    设计模式示例清理脚本
echo ========================================
echo.

if exist build (
    echo 🗑️  删除build目录...
    rmdir /s /q build
    if errorlevel 1 (
        echo ❌ 删除失败
    ) else (
        echo ✅ build目录删除成功
    )
) else (
    echo ℹ️  build目录不存在，无需清理
)

echo.
echo 🧹 清理临时文件...
if exist *.tmp del *.tmp 2>nul
if exist *.log del *.log 2>nul

echo.
echo ========================================
echo    清理完成！
echo ========================================
echo.

echo 按任意键退出...
pause >nul
