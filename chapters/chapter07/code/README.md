# 第7章代码示例

本目录包含第7章《C++科学计算》的所有代码示例。

## 文件说明

### CMake配置示例
- `cmake_basic_example.cmake` - CMakeLists.txt基本结构示例
- `cmake_include_example.cmake` - 查找和包含头文件的方法
- `cmake_static_lib_example.cmake` - 静态库链接示例
- `cmake_shared_lib_example.cmake` - 动态库链接示例
- `cmake_compile_options.cmake` - 编译选项和宏定义设置
- `cmake_conditional.cmake` - 条件编译和平台检测
- `cmake_complete_example.cmake` - 完整项目配置（包含Eigen、Boost、MKL）

### Eigen库示例
- `eigen_data_types.cpp` - Eigen基本数据类型（Matrix、Vector、Array）
- `eigen_matrix_creation.cpp` - 矩阵创建与初始化方法
- `eigen_matrix_arithmetic.cpp` - 矩阵算术运算（加减乘除、元素级运算）
- `eigen_element_access.cpp` - 矩阵元素访问方法
- `eigen_block_operations.cpp` - 矩阵块操作（提取、修改子矩阵）
- `eigen_transpose_conjugate.cpp` - 矩阵转置与共轭
- `eigen_norm_statistics.cpp` - 矩阵范数与统计（范数、迹、行列式等）
- `eigen_matrix_ops.cpp` - 矩阵高级运算示例
- `eigen_solvers.cpp` - Eigen线性方程求解（LU、QR、Cholesky分解）
- `eigen_eigenvalues.cpp` - Eigen特征值和特征向量计算
- `eigen_performance_optimization.cpp` - 性能优化技巧示例（noalias、固定大小矩阵、Map等）
- `eigen_numerical_stability.cpp` - 数值稳定性示例（条件数检查、避免求逆、验证解等）
- `eigen_debugging.cpp` - 调试技巧示例（打印信息、断言检查、NaN/Inf检测等）

### Boost库示例
- `boost_math.cpp` - Boost数学函数（特殊函数、常数等）
- `boost_math_advanced.cpp` - Boost数学高级功能
- `boost_math_comprehensive.cpp` - Boost数学综合示例
- `boost_math_special_functions.cpp` - Boost特殊函数示例
- `boost_multiprecision.cpp` - Boost高精度计算
- `boost_multiprecision_types.cpp` - Boost高精度数据类型
- `boost_statistics.cpp` - Boost统计计算（累加器、分布、假设检验）
- `boost_random.cpp` - Boost随机数生成（多种分布）
- `boost_units.cpp` - Boost单位系统
- `boost_geometry.cpp` - Boost几何算法
- `boost_accumulators_examples.cpp` - Boost累加器示例
- `boost_interval.cpp` - Boost区间算术（误差分析和数值稳定性）

### Intel OneMKL示例

#### 基础模块
- `mkl_blas.cpp` - MKL BLAS使用（矩阵乘法、向量运算等）
- `mkl_lapack.cpp` - MKL LAPACK使用（线性方程求解、特征值、SVD）
- `mkl_lapack_advanced.cpp` - MKL LAPACK高级功能

#### FFT和变换
- `mkl_fft.cpp` - MKL FFT使用（一维、二维、实数FFT）
- `mkl_fft_advanced.cpp` - MKL FFT高级功能
- `mkl_dft.cpp` - MKL DFT接口（离散傅里叶变换，高级特性、批量处理）
- `mkl_fftw3x.cpp` - MKL FFTW3X接口（FFTW兼容接口）

#### 向量数学和随机数
- `mkl_vml.cpp` - MKL VML向量数学库
- `mkl_random.cpp` - MKL随机数生成（VSL）
- `mkl_vsl_essl.cpp` - MKL VSL ESSL兼容接口

#### 稀疏矩阵
- `mkl_sparse.cpp` - MKL稀疏矩阵基础
- `mkl_sparse_blas.cpp` - MKL稀疏BLAS（SpMV、SpMM、矩阵运算）
- `mkl_sparse_directsolvers.cpp` - MKL稀疏直接求解器（Pardiso、LU、Cholesky）
- `mkl_sparse_itersolvers.cpp` - MKL稀疏迭代求解器（CG、BiCG、GMRES、PCG）
- `mkl_sparse_eigsolvers.cpp` - MKL稀疏特征值求解器（Lanczos、Arnoldi）
- `mkl_pardiso.cpp` - MKL Pardiso求解器详细示例

#### 统计和数据拟合
- `mkl_statistics.cpp` - MKL统计函数（描述性统计、相关性、假设检验）
- `mkl_data_fitting.cpp` - MKL数据拟合（线性拟合、多项式拟合、插值、平滑）

#### 非线性求解和PDE
- `mkl_nonlinear_solvers.cpp` - MKL非线性求解器（Newton法、拟Newton法、Levenberg-Marquardt）
- `mkl_pdepoisson.cpp` - MKL PDE Poisson求解器（泊松方程）
- `mkl_pdett.cpp` - MKL PDE TT求解器（时间相关PDE：热方程、波动方程）

#### 工具模块
- `mkl_trans.cpp` - MKL变换模块（矩阵转置、数据布局变换）
- `mkl_service.cpp` - MKL服务模块（版本信息、线程设置、内存管理、性能监控）

### 库混合使用示例
- `eigen_mkl_integration.cpp` - Eigen使用MKL加速（BLAS和LAPACK后端）
- `boost_eigen_integration.cpp` - Boost与Eigen结合使用（高精度计算与矩阵运算）
- `mixed_libraries_example.cpp` - 综合应用示例（三个库协同使用）

## 编译选项

### 通用选项
```bash
-std=c++11    # C++11标准支持
-O3          # 最高优化级别
-march=native # 本地架构优化
```

### Eigen库
只需要包含头文件，无需链接库文件。

### Boost库
```bash
-I/path/to/boost/include
-L/path/to/boost/lib
-lboost_math
-lboost_random
```

### Intel OneMKL
```bash
# Linux
-I${MKLROOT}/include
-L${MKLROOT}/lib/intel64
-lmkl_intel_lp64
-lmkl_core
-lmkl_sequential

# Windows
/I"%MKLROOT%\include"
/LIBPATH:"%MKLROOT%\lib\intel64"
mkl_intel_lp64.lib
mkl_core.lib
mkl_sequential.lib
```

## 运行示例

```bash
# 编译
g++ -std=c++11 -O3 example.cpp -o example

# 运行
./example
```

### 混合使用示例编译

#### Eigen + MKL
```bash
# Linux
g++ -std=c++11 -O3 -DEIGEN_USE_MKL_ALL \
    -I${EIGEN_ROOT} -I${MKLROOT}/include \
    eigen_mkl_integration.cpp \
    -L${MKLROOT}/lib/intel64 \
    -lmkl_intel_lp64 -lmkl_core -lmkl_sequential \
    -lpthread -lm -ldl -o eigen_mkl_integration

# Windows
cl /EHsc /O2 /DEIGEN_USE_MKL_ALL \
   /I"%EIGEN_ROOT%" /I"%MKLROOT%\include" \
   eigen_mkl_integration.cpp \
   /link /LIBPATH:"%MKLROOT%\lib\intel64" \
   mkl_intel_lp64.lib mkl_core.lib mkl_sequential.lib
```

#### Boost + Eigen
```bash
g++ -std=c++11 -O3 \
    -I${EIGEN_ROOT} -I${BOOST_ROOT}/include \
    boost_eigen_integration.cpp \
    -L${BOOST_ROOT}/lib \
    -lboost_math -lboost_random -o boost_eigen_integration
```

#### 综合示例（Eigen + MKL + Boost）
```bash
g++ -std=c++11 -O3 -DEIGEN_USE_MKL_ALL \
    -I${EIGEN_ROOT} -I${MKLROOT}/include -I${BOOST_ROOT}/include \
    mixed_libraries_example.cpp \
    -L${MKLROOT}/lib/intel64 -L${BOOST_ROOT}/lib \
    -lmkl_intel_lp64 -lmkl_core -lmkl_sequential \
    -lboost_math -lboost_random \
    -lpthread -lm -ldl -o mixed_libraries_example
```

## 性能测试

部分示例包含性能计时功能，可用于比较不同方法和库的性能表现。

## 注意事项

1. **内存管理**：注意大型矩阵的内存使用
2. **数值稳定性**：选择合适的方法保证数值稳定性
3. **并行化**：OneMKL支持多线程，可设置环境变量控制线程数
4. **精度控制**：Boost多精度计算可根据需要调整精度

## 扩展阅读

- Eigen文档：http://eigen.tuxfamily.org/
- Boost文档：https://www.boost.org/
- Intel MKL文档：https://software.intel.com/content/www/us/en/develop/tools/math-kernel-library.html
