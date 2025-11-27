# MT反演程序模块化重构说明

## 概述

本次重构将MT反演程序的正演和反演步骤模块化，提高了代码的可维护性和可扩展性。

## 模块结构

### 1. 数据模型模块 (`mt_model.h`)

定义了所有数据结构：
- `InversionParams`: 反演参数
- `InversionResult`: 反演结果
- `ModelParams`: 模型参数
- `FrequencyParams`: 频率参数
- `ObservationData`: 观测数据

**位置**: `MT` 命名空间

### 2. 频率生成器模块 (`mt_frequency_generator.h/cpp`)

负责生成MT反演所需的频率数组。

**主要功能**:
- `generate()`: 生成频率数组（对数均匀分布）
- `generateParams()`: 生成频率参数结构

**特点**:
- 可自定义周期范围
- 支持默认参数

### 3. 正演求解器模块 (`mt_forward_solver.h/cpp`)

封装有限差分法求解1D MT正演问题。

**主要功能**:
- `solve()`: 执行正演计算
- `setBoundaryCondition()`: 设置边界条件类型
- `setUseAverageGridSpacing()`: 设置网格间距计算方式

**特点**:
- 支持多种边界条件（辐射边界、理想导体边界）
- 可配置网格间距计算方式
- 使用MKL库进行高性能计算

### 4. Jacobian计算器模块 (`mt_jacobian_calculator.h/cpp`)

负责计算反演所需的Jacobian矩阵（灵敏度矩阵）。

**主要功能**:
- `compute()`: 计算Jacobian矩阵
- `setPerturbationMethod()`: 设置扰动方法类型

**特点**:
- 支持前向差分和中心差分两种方法
- 依赖正演求解器进行计算

### 5. 正则化模块 (`mt_regularization.h/cpp`)

负责构建正则化矩阵，用于反演中的模型平滑约束。

**主要功能**:
- `buildLMatrix()`: 构建正则化矩阵L
- `computeLTL()`: 计算L^T*L
- `setType()`: 设置正则化类型

**支持的正则化类型**:
- `SMOOTHNESS`: 平滑度约束（二阶差分）
- `FLATNESS`: 平坦度约束（一阶差分）
- `MINIMUM_NORM`: 最小范数约束（单位矩阵）

### 6. 优化求解器模块 (`mt_optimizer.h/cpp`)

负责求解反演中的正规方程。

**主要功能**:
- `solve()`: 求解正规方程
- `computeJTJ()`: 计算J^T*J（使用MKL BLAS）
- `computeJTr()`: 计算J^T*r（使用MKL BLAS）
- `setSolverType()`: 设置求解器类型

**特点**:
- 支持Cholesky分解和LU分解
- 使用MKL BLAS进行矩阵运算

### 7. 核心协调器 (`mt_inversion_core.h/cpp`)

作为协调器，使用各个模块化组件完成反演任务。

**主要功能**:
- `invert()`: 执行反演
- `generateRandomModel()`: 生成随机模型
- `computeLayerThicknesses()`: 计算层厚度

**特点**:
- 保持向后兼容的接口
- 提供模块访问接口，方便高级定制

## 模块依赖关系

```
mt_inversion_core
├── mt_model (数据模型)
├── mt_frequency_generator (频率生成)
├── mt_forward_solver (正演求解)
│   └── mt_model
├── mt_jacobian_calculator (Jacobian计算)
│   ├── mt_model
│   └── mt_forward_solver
├── mt_regularization (正则化)
│   └── mt_model
└── mt_optimizer (优化求解)
    └── mt_model
```

## 使用示例

### 基本使用（保持向后兼容）

```cpp
MTInversionCore core;
MTInversionCore::InversionParams params;
params.M = 40;
params.nFreq = 61;
// ... 设置其他参数

MTInversionCore::InversionResult result = core.invert(params);
```

### 高级定制

```cpp
MTInversionCore core;

// 获取各个模块的指针
MT::ForwardSolver* forwardSolver = core.getForwardSolver();
MT::Regularization* regularization = core.getRegularization();

// 自定义设置
forwardSolver->setBoundaryCondition("perfect_conductor");
regularization->setType(MT::Regularization::Type::FLATNESS);

// 执行反演
MTInversionCore::InversionResult result = core.invert(params);
```

### 直接使用模块

```cpp
// 创建频率生成器
MT::FrequencyGenerator freqGen;
std::vector<double> periods, omega;
freqGen.generate(61, periods, omega);

// 创建正演求解器
MT::ForwardSolver solver;
MT::ModelParams model;
model.mLogRho = {2.0, 2.0, ...};  // log10(ρ)
model.layerThicknesses = {10.0, 12.0, ...};

std::vector<double> dataOut;
solver.solve(model, omega, dataOut);
```

## 扩展指南

### 添加新的正演方法

1. 创建新的正演求解器类（继承或实现 `ForwardSolver` 接口）
2. 在 `MTInversionCore` 中替换或添加新的求解器选项

### 添加新的正则化方法

1. 在 `mt_regularization.h` 中添加新的正则化类型枚举
2. 在 `mt_regularization.cpp` 中实现对应的构建函数

### 添加新的优化算法

1. 在 `mt_optimizer.h` 中添加新的求解器类型
2. 在 `mt_optimizer.cpp` 中实现对应的求解函数

## 优势

1. **模块化**: 每个模块职责单一，易于理解和维护
2. **可扩展**: 可以轻松添加新的正演方法、正则化方法或优化算法
3. **可测试**: 每个模块可以独立测试
4. **向后兼容**: 保持了原有的接口，现有代码无需修改
5. **高性能**: 继续使用MKL库进行高性能计算

## 文件清单

### 新增文件
- `mt_model.h`: 数据模型定义
- `mt_frequency_generator.h/cpp`: 频率生成器
- `mt_forward_solver.h/cpp`: 正演求解器
- `mt_jacobian_calculator.h/cpp`: Jacobian计算器
- `mt_regularization.h/cpp`: 正则化模块
- `mt_optimizer.h/cpp`: 优化求解器

### 修改文件
- `mt_inversion_core.h/cpp`: 重构为核心协调器
- `CMakeLists.txt`: 添加新模块的编译配置

## 注意事项

1. 所有模块都在 `MT` 命名空间中
2. 保持向后兼容性，原有接口仍然可用
3. 模块之间通过接口交互，降低耦合度
4. 使用MKL库时需要正确配置MKLROOT环境变量

