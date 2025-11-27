# 高等程序设计 - Qt/C++ 课程资料

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Qt](https://img.shields.io/badge/Qt-6.0%2B-green.svg)](https://www.qt.io/)

本项目包含长江大学地球物理与石油资源学院《高等程序设计 - Qt/C++》课程的完整教学资料，包括课件、代码示例和实验指导。

> **注意**: 本项目部分开发工作借助了AI工具Cursor，特此说明。

## 📋 项目状态


![GitHub last commit](https://img.shields.io/github/last-commit/EMWPJ/advanced-programming-qt-cpp)
![GitHub repo size](https://img.shields.io/github/repo-size/EMWPJ/advanced-programming-qt-cpp)
![GitHub issues](https://img.shields.io/github/issues/EMWPJ/advanced-programming-qt-cpp)
![GitHub pull requests](https://img.shields.io/github/issues-pr/EMWPJ/advanced-programming-qt-cpp)

## 📚 课程简介

本课程系统介绍了C++编程语言的基础知识和Qt框架的应用开发，旨在培养学生扎实的编程基础和现代GUI应用程序开发能力。

### 🎯 课程目标
- 掌握C++语言的核心语法和面向对象编程思想
- 熟练使用Qt框架开发跨平台GUI应用程序
- 理解软件开发的完整流程和最佳实践
- 培养良好的编程习惯和调试能力

## 📖 课程内容

### 第1章：开发环境搭建
- 软件开发概述
- 开发环境介绍
- Qt Creator安装与配置
- 第一个Qt应用程序
- 项目结构与构建系统

### 第2章：C++语言基础与进阶
- C/C++基础语法
- 面向对象编程
- 模板与泛型编程
- STL标准库
- 智能指针与内存管理
- Lambda表达式与现代C++

### 第3章：Qt框架基础知识
- Qt核心概念
- 信号与槽机制
- 窗口与控件
- 布局管理
- 事件处理

### 第4章：编程范式详解
- 面向对象编程进阶
- 泛型编程
- 函数式编程
- 现代C++特性
- 设计模式基础

### 第5章：设计模式
- 创建型模式（单例、工厂、建造者等）
- 结构型模式（适配器、装饰器、代理等）
- 行为型模式（观察者、策略、命令等）
- 设计原则与最佳实践

### 第6章：Qt高级特性与实战
- Qt Quick/QML
- 数据库操作
- 网络编程
- 多线程编程
- 高级应用开发


## 🏗️ 项目结构

```
chapters/
├── chapter01/          # 第1章：开发环境搭建
│   ├── code/          # 代码示例
│   ├── images/        # 图片资源
│   └── chapter01.tex  # LaTeX课件
├── chapter02/          # 第2章：C++语言基础与进阶
├── chapter03/          # 第3章：Qt框架基础知识
├── chapter04/          # 第4章：编程范式详解
├── chapter05/          # 第5章：设计模式
├── chapter06/          # 第6章：Qt高级特性与实战
├── setup.tex          # LaTeX样式配置
└── rebuild.bat        # 批量重建脚本
```

## 🛠️ 开发环境要求

### 系统要求
- **操作系统**: Windows 10/11, macOS 10.15+, Ubuntu 18.04+
- **内存**: 至少4GB RAM
- **存储**: 至少2GB可用空间

### 软件依赖
- **Qt Creator** 6.0+ 或 **Qt 6.0+**
- **CMake** 3.16+
- **Git** 2.0+
- **LaTeX** (用于编译课件)
  - TeX Live 或 MiKTeX
  - minted包 (需要Python和Pygments)

### 安装指南

#### 1. 安装Qt
```bash
# 下载并安装Qt在线安装器
# https://www.qt.io/download
# 选择Qt 6.x版本，包含Qt Creator
```

#### 2. 安装CMake
```bash
# Windows: 下载安装包
# https://cmake.org/download/

# Ubuntu/Debian:
sudo apt-get install cmake

# macOS:
brew install cmake
```

#### 3. 安装LaTeX (可选，用于编译课件)
```bash
# Windows: 安装MiKTeX
# https://miktex.org/

# Ubuntu/Debian:
sudo apt-get install texlive-full

# macOS:
brew install mactex
```

## 🚀 快速开始

### 克隆项目
```bash
# 请将 EMWPJ 替换为实际的GitHub用户名
git clone https://github.com/EMWPJ/advanced-programming-qt-cpp.git
cd advanced-programming-qt-cpp

# 或者直接克隆主仓库（如果已创建）
# git clone https://github.com/EMWPJ/advanced-programming-qt-cpp.git
```

### 编译第一个示例
```bash
cd chapters/chapter01/code
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 运行示例
```bash
# Windows
.\Release\FirstApp.exe

# Linux/macOS
./FirstApp
```

## 📖 课件编译

本项目使用LaTeX Beamer制作幻灯片课件。

### 编译单个章节
```bash
cd chapters/chapter01
xelatex -shell-escape -interaction=nonstopmode chapter01.tex
```

### 批量编译所有章节
```bash
cd chapters
./rebuild.bat  # Windows
# 或手动执行
for dir in chapter*/; do
    cd "$dir"
    xelatex -shell-escape -interaction=nonstopmode "${dir%/}.tex"
    cd ..
done
```

## 🎯 代码示例说明

每个章节的`code/`目录包含：
- **基础示例**: 演示基本概念和语法
- **进阶示例**: 展示高级特性和最佳实践
- **完整项目**: 综合应用多个知识点
- **CMakeLists.txt**: 构建配置文件

### 示例运行方法
```bash
# 进入示例目录
cd chapters/chapter02/code/cpp_basic_syntax

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake ..
cmake --build . --config Release

# 运行程序
./cpp_basic_syntax
```

## 🤝 贡献指南

欢迎提交Issue和Pull Request来改进本课程资料！

详细的贡献指南请查看 [CONTRIBUTING.md](CONTRIBUTING.md)。

### 快速开始
1. Fork 本仓库
2. 创建特性分支: `git checkout -b feature/new-feature`
3. 提交更改: `git commit -m 'Add new feature'`
4. 推送分支: `git push origin feature/new-feature`
5. 提交 Pull Request

### 代码规范
- 遵循Qt编码规范
- 使用有意义的变量和函数名
- 添加必要的注释
- 确保代码能够正常编译

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 👨‍🏫 授课教师

**王培杰**
长江大学地球物理与石油资源学院
邮箱: 478451384@qq.com

## 🙏 致谢

感谢所有为本课程建设做出贡献的师生！

---

⭐ 如果这个项目对你有帮助，请给它一个星标！
