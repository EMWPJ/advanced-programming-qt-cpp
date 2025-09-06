# 第2章：C++语言基础与进阶

## 📖 章节概述

本章系统介绍C++编程语言的基础知识和高级特性，从C、C++与Qt的关系开始，逐步深入到C语言基础、C++基本语法和面向对象编程。通过本章的学习，学生将掌握现代C++编程的核心技能，并理解编程语言的演进过程。

## 🎯 学习目标

1. **理解编程语言演进**：掌握C、C++、Qt三者的关系和学习路径
2. **掌握C语言基础**：理解底层编程思想和内存管理
3. **熟练C++基本语法**：掌握现代C++的语言特性
4. **精通面向对象编程**：深入理解OOP的核心概念和应用
5. **掌握内存管理**：智能指针、RAII、异常安全

## 📚 主要内容

### 1. C、C++与Qt的关系
- **编程语言演进**: C → C++ → Qt框架的发展历程
- **学习路径**: 正常学习路径与非常规路径对比
- **技术特点**: 每种技术的优势、特点和应用场景
- **协同优势**: C++与Qt的完美结合和性能优势

### 2. C语言基础知识
- **C语言概述**: 特点、优势、应用领域、与C++的关系
- **基本语法**: 数据类型、变量常量、运算符表达式
- **控制结构**: if-else、switch-case、循环结构(while、do-while、for)
- **函数编程**: 函数定义、声明、调用、参数传递方式
- **指针基础**: 指针声明、使用、与变量/数组的关系
- **动态内存**: malloc/free、内存分区、常见错误处理
- **结构体**: 定义、使用、指针访问、应用场景

### 3. C++基本语法
- **语言发展**: C++11/14/17/20版本演进和重大特性
- **数据类型**: 扩展类型、类型安全、类型推导(auto、decltype)
- **变量常量**: 声明、初始化、作用域、存储类、const修饰符
- **控制结构**: 增强循环、异常处理、范围for、结构化绑定
- **函数特性**: 重载、默认参数、内联函数、Lambda表达式
- **数组容器**: 原生数组、std::array、std::vector、初始化列表
- **指针引用**: 智能指针、引用类型、nullptr、内存安全

### 4. C++面向对象编程
- **类和对象**: 定义、实例化、成员访问、this指针
- **封装**: 访问控制、构造函数、析构函数、const成员函数
- **继承**: 单继承、多继承、继承规则、虚函数
- **多态**: 虚函数、纯虚函数、抽象类、运行时多态
- **智能指针**: unique_ptr、shared_ptr、weak_ptr、RAII原则

## 💻 代码示例

### C语言示例
- `c_basic_syntax.c`: C语言基础语法
- `c_control_structures.c`: 控制结构演示
- `c_function_example.c`: 函数定义和调用
- `c_pointer_example.c`: 指针使用示例
- `c_struct_example.c`: 结构体定义和使用

### C++基础示例
- `cpp_basic_syntax.cpp`: C++基础语法
- `cpp_basic_types.cpp`: 基本数据类型
- `cpp_string_example.cpp`: 字符串处理
- `cpp_array_example.cpp`: 数组操作
- `cpp_vector_example.cpp`: STL向量容器

### 面向对象示例
- `cpp_class_example.cpp`: 类定义和使用
- `cpp_inherit_example.cpp`: 继承机制
- `cpp_polymorphism_example.cpp`: 多态实现
- `cpp_multi_inherit_example.cpp`: 多重继承

### 高级特性示例
- `cpp_function_overload.cpp`: 函数重载
- `cpp_template_example.cpp`: 模板编程
- `cpp_lambda_example.cpp`: Lambda表达式
- `cpp_smart_pointer.cpp`: 智能指针
- `cpp_move_semantics.cpp`: 移动语义

## 🚀 运行示例

### 编译单个示例

```bash
# 进入代码目录
cd code

# 编译C程序
gcc c_basic_syntax.c -o c_basic_syntax
./c_basic_syntax

# 编译C++程序
g++ cpp_basic_syntax.cpp -o cpp_basic_syntax
./cpp_basic_syntax

# 或使用CMake
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 批量编译所有示例

```bash
# Linux/macOS
cd code
find . -name "*.c" -o -name "*.cpp" | xargs -I {} g++ {} -o {}

# Windows PowerShell
cd code
Get-ChildItem -Recurse -Include *.c,*.cpp | ForEach-Object {
    $output = $_.BaseName + ".exe"
    g++ $_.FullName -o $output
}
```

## 📁 文件结构

```
chapter02/
├── README.md              # 本文件
├── chapter02.tex          # LaTeX课件源文件
├── chapter02.pdf          # 编译后的课件PDF
├── code/                  # 代码示例目录
│   ├── CMakeLists.txt     # CMake构建配置
│   ├── c_*.c              # C语言示例文件
│   ├── cpp_*.cpp          # C++示例文件
│   └── build/             # 构建目录
├── images/                # 课件图片资源
├── _minted-chapter02/     # LaTeX代码高亮文件
└── chapter02.*            # LaTeX编译辅助文件
```

## 🛠️ 开发环境

### 编译器要求
- **C编译器**: GCC 7.0+ 或 Clang 5.0+
- **C++编译器**: GCC 9.0+ 或 Clang 7.0+ (支持C++17)
- **构建工具**: CMake 3.16+ (推荐) 或 Make

### 推荐配置
```bash
# 检查编译器版本
gcc --version
g++ --version

# 启用C++17标准
g++ -std=c++17 example.cpp -o example
```

## 📖 学习路径建议

### 阶段1: C语言基础 (1-2周)
1. 基本语法和数据类型
2. 控制结构和循环
3. 函数定义和调用
4. 指针和内存管理

### 阶段2: C++基础 (2-3周)
1. C++语法扩展
2. 引用和字符串处理
3. 面向对象基础
4. STL容器初步

### 阶段3: 高级特性 (2-3周)
1. 模板编程深入
2. 智能指针和RAII
3. 异常处理机制
4. 现代C++特性

## 🎓 练习建议

### 基础练习
1. **变量与类型**: 练习不同数据类型的声明和使用
2. **控制结构**: 实现条件判断和循环逻辑
3. **函数设计**: 编写可重用的函数模块

### 面向对象练习
1. **类设计**: 设计具有完整功能的类
2. **继承层次**: 构建合理的继承体系
3. **多态应用**: 使用虚函数实现多态行为

### 高级练习
1. **模板编程**: 实现泛型算法和容器
2. **内存管理**: 使用智能指针管理资源
3. **STL应用**: 熟练使用各种STL组件

## 🔍 常见问题

### 编译错误
- **C++17支持**: 确保编译器支持C++17标准
- **头文件包含**: 检查所有必要的头文件已包含
- **链接错误**: 确认所有依赖库已正确链接

### 运行时错误
- **内存泄漏**: 使用智能指针或RAII管理资源
- **野指针**: 初始化指针变量，避免悬空指针
- **数组越界**: 使用STL容器替代原生数组

## 📈 进阶学习

完成本章后，建议继续学习：
- **设计模式**: 常用设计模式的C++实现
- **并发编程**: C++11多线程和并发特性
- **网络编程**: 套接字编程和网络应用
- **性能优化**: 代码优化和性能分析

## 📞 技术支持

如有编程问题，请：
1. 查看编译器错误信息
2. 使用调试器单步执行代码
3. 参考示例代码的实现方式
4. 查看C++标准库文档

---
**章节作者**: 王培杰
**更新日期**: \today
