/**
 * @file sor_solver.h
 * @brief 逐次超松弛迭代求解器 - 演示改进的迭代方法
 *
 * 这个求解器展示了：
 * - 逐次超松弛（SOR）算法的实现
 * - Gauss-Seidel方法的改进版本
 * - 超松弛因子的选择和优化
 * - 收敛速度的显著提升
 * - 数值迭代的加速技术
 */

#ifndef SOR_SOLVER_H
#define SOR_SOLVER_H

#include "solver_base.h"  // 基类定义
#include "matrix.h"       // 矩阵类
#include "vector.h"       // 向量类
#include <numeric>        // 用于std::accumulate等数值算法
#include <algorithm>      // 用于std::for_each等算法

/**
 * @class SORSolver
 * @brief 逐次超松弛迭代求解器类
 *
 * 算法描述：
 * 逐次超松弛（SOR）方法是Gauss-Seidel方法的改进版本
 * 主要思想：在Gauss-Seidel基础上引入超松弛因子加速收敛
 *
 * 数学原理：
 * - Gauss-Seidel迭代：x^(k+1)[i] = (b[i] - Σ(j=1 to i-1) A[i][j]*x^(k+1)[j] - Σ(j=i+1 to n) A[i][j]*x^(k)[j]) / A[i][i]
 * - SOR迭代：在Gauss-Seidel基础上乘以超松弛因子ω
 * - 迭代公式：x^(k+1)[i] = ω * x_GS^(k+1)[i] + (1-ω) * x^(k)[i]
 *
 * 收敛性：
 * - 对于对角占优矩阵，当1 < ω < 2时收敛
 * - 最优ω的选择可以显著加速收敛
 * - 收敛速度通常快于Gauss-Seidel和Jacobi方法
 *
 * 适用性：
 * - 适用于大型稀疏矩阵
 * - 需要较少的迭代次数
 * - 对内存需求适中
 *
 * 优势：
 * - 收敛速度快
 * - 实现相对简单
 * - 可以通过调整ω优化性能
 *
 * 缺点：
 * - 需要选择合适的ω
 * - 对某些问题可能不稳定
 *
 * 超松弛因子ω：
 * - ω = 1：退化为Gauss-Seidel方法
 * - 1 < ω < 2：超松弛，加速收敛
 * - ω > 2或ω < 1：可能发散
 * - 最优ω ≈ 2 / (1 + sqrt(1 - ρ^2))，其中ρ为谱半径
 *
 * 演示：
 * - 迭代加速技术
 * - 参数调优的重要性
 * - Gauss-Seidel方法的改进
 */
class SORSolver : public SolverBase {
public:
    /**
     * @brief 构造函数
     *
     * @param omega 超松弛因子（默认1.2）
     * @param tolerance 收敛容差（默认1e-6）
     * @param max_iterations 最大迭代次数（默认1000）
     *
     * 参数选择：
     * - omega：典型值1.0-1.8，取决于矩阵特征
     * - tolerance：控制解的精度
     * - max_iterations：防止无限循环
     *
     * 经验法则：
     * - 对角占优矩阵：ω ≈ 1.2-1.5
     * - 弱对角占优：ω ≈ 1.0-1.2
     * - 实际应用中需要实验调优
     */
    SORSolver(double omega = 1.2, double tolerance = 1e-6, size_t max_iterations = 1000);

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
     * 3. 迭代求解：结合Gauss-Seidel和超松弛
     * 4. 检查收敛性
     * 5. 返回解向量或nullptr
     *
     * 迭代特点：
     * - 使用当前迭代已更新的值
     * - 引入超松弛因子加速收敛
     * - 比Jacobi方法收敛更快
     */
    std::unique_ptr<Vector> solve(const Matrix& A, const Vector& b) override;


private:
    double m_omega;              // 超松弛因子
    double m_tolerance;          // 收敛容差
    size_t m_max_iterations;     // 最大迭代次数

};

#endif // SOR_SOLVER_H
