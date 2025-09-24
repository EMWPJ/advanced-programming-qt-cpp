/**
 * @file jacobi_solver.h
 * @brief 雅可比迭代求解器 - 演示迭代求解方法
 *
 * 这个求解器展示了：
 * - 雅可比迭代算法的实现
 * - 迭代求解方法的基本原理
 * - 收敛性判断和停止准则
 * - 对角占优矩阵的快速收敛
 * - 数值迭代的误差控制
 */

#ifndef JACOBI_SOLVER_H
#define JACOBI_SOLVER_H

#include "solver_base.h"  // 基类定义
#include "matrix.h"       // 矩阵类
#include "vector.h"       // 向量类
#include <numeric>        // 用于std::accumulate等数值算法
#include <algorithm>      // 用于std::for_each等算法

/**
 * @class JacobiSolver
 * @brief 雅可比迭代求解器类
 *
 * 算法描述：
 * 雅可比迭代法是最经典的迭代求解方法之一
 * 主要思想：使用前一次迭代的解来计算当前解
 *
 * 数学原理：
 * - 将方程组A*x = b重写为x = D^(-1)*(b - (L+U)*x)
 * - 其中D是对角矩阵，L是严格下三角，U是严格上三角
 * - 迭代公式：x^(k+1)[i] = (b[i] - Σ(j≠i) A[i][j]*x^(k)[j]) / A[i][i]
 *
 * 收敛性：
 * - 对于对角占优矩阵保证收敛
 * - 收敛速度与对角占优程度相关
 * - 条件数影响收敛速度
 *
 * 适用性：
 * - 适用于大型稀疏矩阵
 * - 适合并行计算（各分量独立）
 * - 需要较多次迭代
 *
 * 优势：
 * - 实现简单
 * - 内存需求固定
 * - 易于并行化
 *
 * 缺点：
 * - 收敛速度较慢
 * - 不如Gauss-Seidel方法有效
 *
 * 演示：
 * - 迭代算法的实现
 * - 收敛性控制
 * - 数值精度管理
 */
class JacobiSolver : public SolverBase {
public:
    /**
     * @brief 构造函数
     *
     * @param tolerance 收敛容差（默认1e-6）
     * @param max_iterations 最大迭代次数（默认1000）
     *
     * 设计考虑：
     * - 默认参数提供合理的数值精度
     * - 最大迭代次数防止无限循环
     * - 容差控制数值精度
     */
    JacobiSolver(double tolerance = 1e-6, size_t max_iterations = 1000);

    /**
     * @brief 求解线性方程组
     *
     * @param A 系数矩阵
     * @param b 常数向量
     * @return 解向量，如果失败返回nullptr
     *
     * 算法流程：
     * 1. 验证输入参数
     * 2. 初始化解向量（通常为零向量）
     * 3. 迭代求解直到收敛或达到最大迭代次数
     * 4. 返回解向量或nullptr
     *
     * 迭代过程：
     * - 使用前一次迭代的解计算新解
     * - 检查收敛性
     * - 避免使用当前迭代的解更新其他分量
     */
    std::unique_ptr<Vector> solve(const Matrix& A, const Vector& b) override;


private:
    double m_tolerance;           // 收敛容差
    size_t m_max_iterations;      // 最大迭代次数

};

#endif // JACOBI_SOLVER_H
