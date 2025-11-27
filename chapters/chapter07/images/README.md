# 第7章图片资源

本目录用于存放第7章《C++科学计算》的图片资源。

## 结构图说明

本章包含三个科学计算库的结构图，采用**总分结构**：
- **总图**：展示库的整体结构和主要模块
- **分图**：展示每个主要模块的详细结构

## 文件结构

### Eigen线性代数库
- `eigen_overview.dot` - 总图：展示Core、Dense、Sparse、Geometry、Eigenvalues等主要模块
- `eigen_dense.dot` - Dense模块详细结构（Matrix/Array/Vector类型、基础运算、块操作、Map类型、矩阵分解、表达式模板、SIMD优化等）
- `eigen_sparse.dot` - Sparse模块详细结构（存储格式、稀疏矩阵运算等）
- `eigen_geometry.dot` - Geometry模块详细结构（旋转、平移、缩放等）
- `eigen_eigenvalues.dot` - Eigenvalues模块详细结构（特征值求解器等）

### Boost科学计算库
- `boost_overview.dot` - 总图：展示Math、Multiprecision、Accumulators、Random、Units、Geometry、Interval、UUID等组件
- `boost_math.dot` - Math模块详细结构（特殊函数、数值积分等）
- `boost_multiprecision.dot` - Multiprecision模块详细结构（高精度整数/浮点数等）
- `boost_accumulators.dot` - Accumulators模块详细结构（统计累加器等）
- `boost_random.dot` - Random模块详细结构（随机数生成器、分布等）
- `boost_units.dot` - Units模块详细结构（单位系统等）
- `boost_geometry.dot` - Geometry模块详细结构（几何算法等）
- `boost_interval.dot` - Interval模块详细结构（区间算术等）

### Intel OneMKL库
- `mkl_overview.dot` - 总图：展示所有MKL组件（BLAS、LAPACK、FFT、DFT、FFTW3X、Pardiso、VML、VSL、VSL_ESSL、稀疏BLAS、稀疏直接/迭代/特征值求解器、统计函数、数据拟合、非线性求解器、PDE求解器、Trans、Service等）
- `mkl_blas.dot` - BLAS模块详细结构（Level 1/2/3等）
- `mkl_lapack.dot` - LAPACK模块详细结构（矩阵分解、特征值等）
- `mkl_fft.dot` - FFT模块详细结构（快速傅里叶变换等）
- `mkl_dft.dot` - DFT模块详细结构（离散傅里叶变换接口等）
- `mkl_fftw3x.dot` - FFTW3X模块详细结构（FFTW接口兼容等）
- `mkl_pardiso.dot` - Pardiso模块详细结构（三阶段求解流程、迭代求解器等）
- `mkl_vml.dot` - VML模块详细结构（向量数学函数等）
- `mkl_vsl.dot` - VSL模块详细结构（随机数生成器等）
- `mkl_vsl_essl.dot` - VSL_ESSL模块详细结构（ESSL兼容接口等）
- `mkl_sparse_blas_overview.dot` - 稀疏BLAS模块概览图（展示主要模块：Level 1、Level 2/3、Inspector-Executor、格式转换、稀疏QR等）
- `mkl_sparse_blas_level1.dot` - Level 1稀疏向量操作（?axpyi, ?doti, ?gthr, ?sctr等8个函数）
- `mkl_sparse_blas_level2_3.dot` - Level 2/3传统接口（矩阵-向量乘法、矩阵-矩阵乘法、三角求解等，支持CSR/BSR/COO/DIA/SKY等格式）
- `mkl_sparse_blas_inspector_executor.dot` - Inspector-Executor接口（两阶段工作流程：Inspector阶段矩阵管理和优化提示，Executor阶段执行运算）
- `mkl_sparse_blas_format_convert.dot` - 格式转换（稠密、CSR、CSC、COO、BSR、DIA、SKY等格式之间的转换）
- `mkl_sparse_blas_qr.dot` - 稀疏QR分解（QR分解流程、QR求解、Q乘法、R求解等）
- `mkl_sparse_blas.dot` - 稀疏BLAS模块完整结构图（包含所有功能的完整版本，适合参考）
- `mkl_sparse_directsolvers.dot` - 稀疏直接求解器模块详细结构（Pardiso、稀疏LU/QR/Cholesky等）
- `mkl_sparse_eigsolvers.dot` - 稀疏特征值求解器模块详细结构（Lanczos、Arnoldi、Jacobi-Davidson等）
- `mkl_sparse_itersolvers.dot` - 稀疏迭代求解器模块详细结构（CG、BiCG、GMRES等）
- `mkl_statistics.dot` - 统计函数模块详细结构（统计量计算等）
- `mkl_data_fitting.dot` - 数据拟合模块详细结构（曲线拟合、插值等）
- `mkl_nonlinear_solvers.dot` - 非线性求解器模块详细结构（Newton法、拟Newton法等）
- `mkl_pdepoisson.dot` - PDE Poisson模块详细结构（泊松方程求解器等）
- `mkl_pdett.dot` - PDE TT模块详细结构（时间相关PDE求解器等）
- `mkl_trans.dot` - Trans模块详细结构（矩阵转置、变换等）
- `mkl_service.dot` - Service模块详细结构（服务函数、工具函数等）

## 生成PDF

运行 `generate_pdfs.bat` 脚本可以将所有 `.dot` 文件转换为 `.pdf` 文件。

**要求**：需要安装 Graphviz（包含 `dot` 命令）

### 安装Graphviz
- Windows: 从 https://graphviz.org/download/ 下载安装
- Linux: `sudo apt-get install graphviz`
- macOS: `brew install graphviz`

## 图片格式

- 源文件：`.dot` (Graphviz格式)
- 输出格式：`.pdf` (用于LaTeX文档)
- 矢量图格式，可无损缩放

## 命名规范

- 总图：`{库名}_overview.dot`
- 分图：`{库名}_{模块名}.dot`
- 使用英文命名，描述性名称
