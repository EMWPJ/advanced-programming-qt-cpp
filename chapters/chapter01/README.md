# 第1章：开发环境搭建

## 📖 章节概述

本章从编程概念入门开始，系统介绍软件开发的整体概念、开发环境的搭建和配置、Qt开发环境的安装和使用，以及多种集成开发环境（Qt Creator、Visual Studio、VS Code）的配置方法。通过本章的学习，学生将掌握编程的基本概念和完整的Qt开发环境搭建技能。

## 🎯 学习目标

1. **理解编程基础概念**：掌握编程的基本原理和思维方式
2. **理解软件开发流程**：掌握软件开发的完整生命周期和规范
3. **掌握开发环境搭建**：能够独立配置Qt开发环境
4. **熟练使用Qt Creator**：掌握Qt官方IDE的使用方法
5. **掌握编译器配置**：了解C++编译过程和调试工具配置
6. **熟悉版本控制**：掌握Git的基本使用和集成
7. **掌握项目实践**：能够创建和运行Qt应用程序
8. **多IDE支持**：能够在Visual Studio和VS Code中开发Qt应用

## 📚 主要内容

### 1. 软件开发概述
- 软件开发的基本概念和流程
- 开发工具和编程语言介绍
- 软件框架的选择和应用
- 开发环境的定义和重要性

### 2. 开发环境概述
- 开发环境的核心组成和流程
- 编译器、框架、IDE、调试工具
- 版本控制系统的集成

### 3. Qt开发环境安装
- Qt框架简介和优势
- Qt安装方式（在线安装器、离线安装包）
- 系统要求和准备工作
- 详细的安装步骤和配置

### 4. Qt Creator IDE
- Qt Creator简介和特性
- 界面布局和区域功能
- 代码编辑、项目管理和调试功能
- 快捷键和使用技巧

### 5. 编译器配置
- 编译器的工作原理
- C++编译过程（预处理、编译、汇编、链接）
- 支持的编译器（MinGW、MSVC、Clang）
- 配置方法和验证

### 6. 调试工具配置
- 调试器的类型和选择
- 调试功能（断点、变量监视、调用栈）
- 配置方法和使用技巧

### 7. 版本控制集成
- Git的基本概念和安装
- Git的基本配置和常用操作
- 在IDE中集成Git
- 分支管理和冲突解决

### 8. 项目模板和示例
- Qt项目模板介绍
- 第一个Qt Widgets项目
- 项目结构和文件组织

### 9. 集成开发环境介绍
- IDE的概念和优势
- 主流IDE对比（Qt Creator、Visual Studio、VS Code）

### 10. Qt在Visual Studio中的配置
- Visual Studio安装和配置
- Qt VS Tools扩展安装
- Qt项目开发流程

### 11. Qt在VS Code中的配置
- VS Code安装和基础配置
- 必要扩展的安装
- Qt项目结构和管理
- 开发流程和调试配置

### 12. 环境验证
- 环境配置验证步骤
- 常见问题及解决方案
- 总结和课后任务

## 💻 代码示例

本章包含以下代码示例：

- **FirstApp**: 第一个Qt应用程序示例
  - 演示基本的Qt窗口创建
  - 展示Qt项目的文件结构
  - 包含完整的构建配置

### 运行示例

```bash
# 进入代码目录
cd code

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译项目
cmake --build . --config Release

# 运行程序
./FirstApp  # Linux/macOS
# 或
FirstApp.exe  # Windows
```

## 📁 文件结构

```
chapter01/
├── README.md              # 本文件
├── chapter01.tex          # LaTeX课件源文件
├── chapter01.pdf          # 编译后的课件PDF
├── code/                  # 代码示例
│   ├── CMakeLists.txt     # CMake构建配置
│   ├── main.cpp           # 主程序文件
│   └── preproc.cpp        # 预处理示例
├── images/                # 课件图片资源
│   └── qt_logo.png        # Qt logo
└── _minted-chapter01/     # LaTeX代码高亮文件
```

## 🛠️ 开发环境要求

### 必备软件
- **Qt Creator** 6.0+ 或 **Qt 6.0+**
- **CMake** 3.16+
- **C++编译器** (MSVC/Clang/GCC)

### 推荐配置
- **操作系统**: Windows 10/11, macOS 10.15+, Ubuntu 18.04+
- **内存**: 至少4GB RAM
- **存储**: 至少2GB可用空间

## 📖 课件说明

- **格式**: 使用Beamer制作的16:9幻灯片
- **主题**: Madrid主题，采用长江大学蓝配色
- **字体**: Source Sans Pro (正文字体), Source Code Pro (代码字体)
- **特色**: 支持中文显示，包含代码高亮和动画效果

## 🎓 练习建议

1. **环境搭建练习**: 独立安装和配置Qt开发环境
2. **项目创建练习**: 使用Qt Creator创建不同类型的项目
3. **代码修改练习**: 修改示例代码，观察运行结果变化
4. **调试练习**: 学习使用Qt Creator的调试功能

## 🔗 相关链接

- [Qt官方下载](https://www.qt.io/download)
- [Qt Creator文档](https://doc.qt.io/qtcreator/)
- [CMake教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

## 📞 技术支持

如遇到环境配置问题，请：
1. 查看Qt官方安装指南
2. 检查系统兼容性要求
3. 参考本章课件中的详细步骤
4. 联系课程助教获取帮助

---
**章节作者**: 王培杰
**更新日期**: \today
