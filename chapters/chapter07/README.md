# 第7章：C++科学计算

本章介绍了C++中常用的科学计算库，包括Eigen、Boost和Intel OneMKL，以及如何安装、配置和使用这些外部库。

## 内容概述

### 外部库的安装与使用
- 外部库概述（静态库、动态库、头文件库）
- 库的安装方法（Eigen、Boost、OneMKL）
- CMakeLists.txt配置
- 静态库和动态库的使用
- 编译参数传递和条件编译
- 完整项目配置示例

### Eigen线性代数库
- 轻量级C++模板库
- 支持向量、矩阵运算
- 编译时尺寸检查
- 高效的表达式模板
- 安装与配置方法

### Boost科学计算库
- 准标准C++库
- 包含数学函数、高精度计算、统计计算、随机数生成等组件
- 注重代码质量和性能
- 静态/动态链接配置

### Intel OneMKL库
- Intel Math Kernel Library
- 高度优化的数学函数
- 支持多线程并行
- BLAS、LAPACK、FFT等功能
- 环境变量配置和链接方法

### 库混合使用
- Eigen使用MKL加速
- Boost与Eigen结合
- 综合应用示例

### 最佳实践与性能优化
- 性能优化策略（编译优化、代码优化、内存优化）
- 数值稳定性建议（条件数检查、误差控制）
- 内存管理最佳实践

### 常见问题与解决方案
- 编译问题（找不到头文件、链接错误、CMake配置）
- 运行时问题（维度不匹配、数值问题、性能问题）
- 调试技巧（调试工具、调试方法、验证技巧）

## 代码示例

所有示例代码位于`code/`目录下：

### CMake配置示例
- `cmake_basic_example.cmake` - CMakeLists.txt基本结构
- `cmake_include_example.cmake` - 查找和包含头文件
- `cmake_static_lib_example.cmake` - 静态库链接
- `cmake_shared_lib_example.cmake` - 动态库链接
- `cmake_compile_options.cmake` - 编译选项设置
- `cmake_conditional.cmake` - 条件编译和宏定义
- `cmake_complete_example.cmake` - 完整项目配置示例

### C++代码示例
- `eigen_*.cpp` - Eigen库使用示例
- `boost_*.cpp` - Boost库使用示例
- `mkl_*.cpp` - Intel OneMKL使用示例
- `*_integration.cpp` - 库混合使用示例

## 编译和运行

### LaTeX文档编译
由于使用了minted包进行代码高亮，需要安装Pygments并使用-shell-escape选项：

```bash
# 安装Pygments（如果还没有安装）
pip install Pygments

# 在chapters/chapter07目录中编译
cd chapters/chapter07
xelatex -shell-escape -interaction=nonstopmode chapter07.tex
xelatex -shell-escape -interaction=nonstopmode chapter07.tex  # 第二次编译处理交叉引用

# 或者使用项目提供的编译脚本
cd chapters
rebuild.bat  # 编译所有章节
```

### C++代码编译

#### Eigen库
```bash
g++ -std=c++11 -I/path/to/eigen eigen_basic.cpp -o eigen_basic
./eigen_basic
```

#### Boost库
```bash
g++ -std=c++11 -I/path/to/boost boost_math.cpp -lboost_math -o boost_math
./boost_math
```

#### Intel OneMKL
```bash
# Linux
g++ -std=c++11 -I${MKLROOT}/include mkl_blas.cpp -L${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -ldl -o mkl_blas
./mkl_blas

# Windows
cl /EHsc /I"%MKLROOT%\\include" mkl_blas.cpp /link /LIBPATH:"%MKLROOT%\\lib" mkl_intel_lp64.lib mkl_core.lib mkl_sequential.lib
mkl_blas.exe
```

#### 使用CMake构建所有示例
```bash
cd chapters/chapter07/code
mkdir build
cd build
cmake ..
make
```

## 注意事项

1. **依赖库安装**：确保已安装相应库的头文件和库文件
2. **环境变量**：Intel OneMKL需要正确设置环境变量
3. **编译器支持**：需要支持C++11或更高版本
4. **性能优化**：实际使用中可根据需要启用编译器优化选项

## 学习建议

1. 从基本使用开始，逐步深入高级功能
2. 注意不同库的适用场景和性能特点
3. 结合具体应用需求选择合适的库
4. 关注数值稳定性和精度控制
5. 进行性能测试和优化
