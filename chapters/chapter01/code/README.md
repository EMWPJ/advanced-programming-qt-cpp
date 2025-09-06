# 第1章代码示例

本目录包含第1章"开发环境搭建"的配套代码示例。

## 📋 示例列表

### FirstApp - 第一个Qt应用程序
演示基本的Qt窗口创建和应用程序结构。

**文件说明**:
- `main.cpp`: 主程序入口，创建Qt应用程序实例
- `preproc.cpp`: 预处理指令示例（可选）

**构建运行**:
```bash
# 方法1: 使用CMake
mkdir build && cd build
cmake ..
cmake --build . --config Release
./FirstApp

# 方法2: 直接编译
g++ -std=c++17 main.cpp -o FirstApp $(pkg-config --cflags --libs Qt6Core Qt6Widgets)
./FirstApp
```

## 🛠️ 构建环境

### 依赖要求
- **Qt 6.0+**: 核心库和Widgets模块
- **CMake 3.16+**: 构建系统
- **C++17编译器**: GCC 9+/Clang 7+/MSVC 2019+

### Qt环境配置
确保Qt库路径正确设置：
```bash
# Linux
export CMAKE_PREFIX_PATH="/opt/Qt/6.x.x/gcc_64/lib/cmake/Qt6"

# Windows PowerShell
$env:CMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2019_64\lib\cmake\Qt6"
```

## 📖 代码说明

### main.cpp 分析
```cpp
#include <QApplication>    // Qt应用程序框架
#include <QLabel>          // 标签控件
#include <QWidget>         // 基础窗口控件

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // 创建应用程序对象

    QLabel label("Hello, Qt!");    // 创建标签控件
    label.show();                  // 显示控件

    return app.exec();             // 进入事件循环
}
```

**关键点**:
1. 每个Qt程序都需要QApplication实例
2. GUI控件需要调用show()方法才能显示
3. app.exec()启动事件循环，程序开始运行

### 预处理示例 (preproc.cpp)
演示C/C++预处理指令的使用：
- 宏定义和条件编译
- 头文件包含
- 编译时条件判断

## 🎯 学习要点

1. **Qt应用程序结构**: 理解Qt程序的基本框架
2. **窗口显示机制**: 掌握控件显示的基本方法
3. **构建系统**: 熟悉CMake构建Qt项目的方法
4. **事件循环**: 理解Qt的事件驱动编程模型

## 🚨 常见问题

### 编译错误
- **找不到Qt头文件**: 检查Qt安装路径和环境变量
- **链接错误**: 确保链接了所有必需的Qt库
- **版本不匹配**: 确认Qt版本与编译器兼容

### 运行错误
- **窗口不显示**: 确保调用了show()方法
- **程序立即退出**: 检查是否正确调用了app.exec()
- **中文显示乱码**: 设置正确的编码格式

## 🔧 调试技巧

1. **使用Qt Creator**: 集成的开发环境，方便调试
2. **命令行调试**: 使用gdb进行调试
3. **日志输出**: 使用qDebug()输出调试信息
4. **断点调试**: 在关键位置设置断点

## 📈 扩展练习

1. **修改窗口标题**: 设置应用程序窗口的标题
2. **添加更多控件**: 在窗口中添加按钮、文本框等控件
3. **自定义样式**: 学习设置控件的外观样式
4. **事件处理**: 添加按钮点击事件的处理逻辑

## 📚 相关资源

- [Qt官方文档](https://doc.qt.io/qt-6/qapplication.html)
- [Qt Creator使用指南](https://doc.qt.io/qtcreator/)
- [CMake教程](https://cmake.org/cmake/help/latest/)

## 📞 技术支持

如遇问题，请：
1. 检查编译错误信息
2. 参考Qt官方示例
3. 查看本章课件说明
4. 联系课程助教

---
**示例维护**: 王培杰
**更新日期**: \today
