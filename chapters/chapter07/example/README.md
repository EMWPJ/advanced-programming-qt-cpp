# 大地电磁（MT）一维反演示例程序

## 简介

这是一个完整的大地电磁（MT）一维反演示例程序，演示了如何使用Intel MKL库进行高性能科学计算。

## 功能特点

- **物理设定**：20层地下模型，61个频点（周期0.001秒到1000秒）
- **正演方法**：1D有限差分法求解TE极化Helmholtz方程
- **反演算法**：阻尼高斯-牛顿法（Tikhonov正则化）
- **MKL函数使用**：
  - `LAPACKE_zgtsv`：复数三对角求解器（正演）
  - `cblas_dsyrk`：计算J^T J（对称矩阵）
  - `cblas_dgemv`：计算J^T r
  - `LAPACKE_dposv`：求解对称正定正规方程

## 编译要求

### 必需依赖

1. **CMake** (版本 >= 3.16)
2. **C/C++编译器** (GCC/Clang/MSVC)
3. **Intel MKL** (Intel Math Kernel Library)
4. **Qt5/Qt6** (可选，用于GUI版本)

### MKL安装

#### Windows
1. 下载并安装 [Intel oneAPI Base Toolkit](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html)
2. 设置环境变量 `MKLROOT` 指向MKL安装目录
   - 例如：`C:\Program Files\Intel\oneAPI\mkl\latest`

#### Linux
```bash
# Ubuntu/Debian
sudo apt-get install libmkl-dev

# 或从Intel官网下载安装包
# 设置环境变量 MKLROOT
export MKLROOT=/opt/intel/mkl
```

#### macOS
```bash
brew install mkl
# 或从Intel官网下载安装包
```

## 编译步骤

### 1. 创建构建目录

```bash
cd chapters/chapter07/example
mkdir build
cd build
```

### 2. 配置CMake

#### Windows (使用Visual Studio)
```bash
cmake .. -G "Visual Studio 17 2022" -A x64
```

#### Linux/macOS
```bash
cmake ..
```

### 3. 编译

#### Windows (Visual Studio)
```bash
cmake --build . --config Release
```

#### Linux/macOS
```bash
make -j4
```

## 运行

### 命令行版本

#### Windows
```bash
.\Release\mt1d_inversion.exe
```

#### Linux/macOS
```bash
./mt1d_inversion
```

### GUI版本（如果已编译）

#### Windows
```bash
.\Release\mt1d_inversion_gui.exe
```

#### Linux/macOS
```bash
./mt1d_inversion_gui
```

GUI版本提供了友好的图形界面，包括：
- **参数设置面板**：可以调整反演参数
- **实时进度显示**：显示迭代进度和残差
- **结果表格**：显示各层的真实模型、初始模型和反演结果
- **日志输出**：显示反演过程的详细信息

## 输出说明

程序会输出以下信息：

1. **频率数组生成**：显示频率范围和周期范围
2. **模型设置**：真实模型、初始模型
3. **反演迭代过程**：
   - 每次迭代的残差L2范数
   - 模型更新范数
4. **最终结果**：
   - 真实模型（log10(ρ)和ρ）
   - 初始模型（log10(ρ)和ρ）
   - 反演结果（log10(ρ)和ρ）

## 程序参数

可以在源代码中修改以下常量：

- `M`：模型层数（默认20）
- `NFREQ`：频率点数（默认61）
- `EPSILON`：Jacobian扰动步长（默认1e-5）
- `MAX_ITER`：最大迭代次数（默认20）
- `TOL_DM`：模型更新范数容差（默认1e-4）
- `LAMBDA`：正则化参数（默认1.0）

## 故障排除

### MKL未找到

如果CMake提示找不到MKL：

1. 确认MKL已正确安装
2. 设置环境变量 `MKLROOT`：
   ```bash
   # Windows (PowerShell)
   $env:MKLROOT = "C:\Program Files\Intel\oneAPI\mkl\latest"

   # Linux/macOS
   export MKLROOT=/opt/intel/mkl
   ```
3. 重新运行CMake配置

### 链接错误

如果遇到链接错误，检查：

1. MKL库路径是否正确
2. 编译器架构是否匹配（x64/intel64）
3. 运行时库路径是否在系统PATH中

## 参考资料

- [Intel MKL文档](https://software.intel.com/content/www/us/en/develop/documentation/mkl-developer-reference-c/top.html)
- [CMake文档](https://cmake.org/documentation/)

## 许可证

本示例程序仅供学习和研究使用。
