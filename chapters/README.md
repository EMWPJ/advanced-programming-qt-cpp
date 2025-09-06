# 课程章节

本目录包含《高等程序设计 - Qt/C++》课程的所有教学资料，按照章节组织。

## 📚 章节概述

| 章节 | 主题 | 内容概述 |
|------|------|----------|
| [Chapter 01](./chapter01/) | 开发环境搭建 | Qt开发环境安装、配置和第一个应用程序 |
| [Chapter 02](./chapter02/) | C++语言基础与进阶 | C/C++语法、面向对象编程、STL、智能指针 |
| [Chapter 03](./chapter03/) | Qt框架基础知识 | Qt核心概念、信号与槽、窗口控件、布局管理 |
| [Chapter 04](./chapter04/) | 编程范式详解 | 面向对象、泛型编程、函数式编程、设计模式 |
| [Chapter 05](./chapter05/) | 设计模式 | 创建型、结构型、行为型模式及应用 |
| [Chapter 06](./chapter06/) | Qt高级特性与实战 | Qt Quick/QML、数据库、网络、多线程 |

## 📁 目录结构

```
chapters/
├── README.md              # 本文件
├── setup.tex              # LaTeX样式配置
├── rebuild.bat            # 批量重建脚本
├── 长江大学校徽.pdf        # 学校校徽
├── chapter01/             # 第1章：开发环境搭建
├── chapter02/             # 第2章：C++语言基础与进阶
├── chapter03/             # 第3章：Qt框架基础知识
├── chapter04/             # 第4章：编程范式详解
├── chapter05/             # 第5章：设计模式
└── chapter06/             # 第6章：Qt高级特性与实战
```

## 🎯 每个章节包含

每个章节文件夹包含以下内容：

- **课件PDF**: `chapterXX.pdf` - 使用Beamer制作的幻灯片课件
- **LaTeX源文件**: `chapterXX.tex` - 课件的LaTeX源代码
- **代码示例**: `code/` - 配套的C++代码示例
- **图片资源**: `images/` - 课件中使用的图片文件
- **编译文件**: 各种LaTeX编译生成的辅助文件

## 🚀 快速导航

### 学习路径建议

1. **新手入门**: 从Chapter 01开始，熟悉开发环境
2. **基础学习**: Chapter 02深入学习C++语言特性
3. **Qt入门**: Chapter 03开始Qt框架的学习
4. **进阶应用**: Chapter 04-05学习高级特性和项目实践
5. **扩展学习**: Chapter 06探索Qt的高级应用领域

### 代码实践

每个章节的`code/`目录都包含：
- 基础示例：演示基本概念
- 进阶示例：展示高级用法
- 完整项目：综合应用

## 🛠️ 工具和资源

### 编译课件

```bash
# 单个章节编译
cd chapter01
latexmk -pdf chapter01.tex

# 或使用批量脚本
./rebuild.bat  # Windows
```

### 运行代码示例

```bash
# 进入示例目录
cd chapter01/code

# 构建和运行
mkdir build && cd build
cmake ..
cmake --build . --config Release
./FirstApp
```

## 📋 教学资源

- **课件**: Beamer制作的专业幻灯片
- **代码**: 完整的C++示例代码
- **图片**: 精心设计的插图和图表
- **配置**: 统一的LaTeX样式配置

## 🤝 贡献

欢迎提交新的示例代码、改进课件内容或修复问题。请参考根目录的[贡献指南](../CONTRIBUTING.md)。

## 📞 联系方式

如有问题或建议，请联系：
- **王培杰** (课程负责人)
- Email: 478451384@qq.com
