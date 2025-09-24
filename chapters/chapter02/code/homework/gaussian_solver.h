/**
 * @file gaussian_solver.h
 * @brief 高斯消元求解器 - 演示直接求解方法
 *
 * 这个求解器展示了：
 * - 高斯消元算法的实现
 * - 直接求解方法的基本原理
 * - 数值稳定性问题（主元消去）
 * - 部分选主元策略
 * - 回代求解过程
 */

#ifndef GAUSSIAN_SOLVER_H
#define GAUSSIAN_SOLVER_H

#include "solver_base.h"  // 基类定义
#include "matrix.h"       // 矩阵类
#include "vector.h"       // 向量类
#include <algorithm>      // 用于std::swap_ranges等算法

/**
 * @class GaussianSolver
 * @brief 高斯消元求解器类
 *
 * 算法描述：
 * 高斯消元法是求解线性方程组最经典的直接方法之一
 * 主要步骤：
 * 1. 前向消元：将系数矩阵转换为上三角矩阵
 * 2. 回代求解：从三角矩阵得到解向量
 *
 * 数学原理：
 * - 消元过程：使用行操作将矩阵转换为行阶梯形式
 * - 回代过程：从最后一个方程开始向前求解
 * - 数值稳定性：通过选主元避免除零错误
 *
 * 适用性：
 * - 适用于中小型稠密矩阵
 * - 不适合病态矩阵（条件数很大）
 * - 时间复杂度：O(n^3)
 *
 * 演示：
 * - 继承和多态性
 * - 算法的模块化设计
 * - 数值计算的误差控制
 */
class GaussianSolver : public SolverBase {
public:
    /**
     * @brief 求解线性方程组
     *
     * @param A 系数矩阵
     * @param b 常数向量
     * @return 解向量，如果失败返回nullptr
     *
     * 算法流程：
     * 1. 复制输入数据（避免修改原数据）
     * 2. 执行前向消元
     * 3. 如果消元成功，执行回代求解
     * 4. 返回解向量或nullptr
     */
    std::unique_ptr<Vector> solve(const Matrix& A, const Vector& b) override;


private:
    /**
     * @brief 前向消元过程
     *
     * @param A 系数矩阵（会被修改）
     * @param b 常数向量（会被修改）
     * @return 如果消元成功返回true
     *
     * 算法：
     * - 对角线元素归一化
     * - 消去下三角元素
     * - 部分选主元避免除零
     *
     * 数值技巧：
     * - 选主元：选择绝对值最大的元素作为主元
     * - 避免浮点数误差积累
     * - 检测零主元（奇异矩阵）
     */
    bool forward_elimination(Matrix& A, Vector& b);

    /**
     * @brief 回代求解过程
     *
     * @param A 上三角系数矩阵
     * @param b 常数向量
     * @param x 解向量（输出参数）
     *
     * 算法：
     * - 从最后一个方程开始向前求解
     * - 每个变量的解依赖于前面已求解的变量
     * - 时间复杂度：O(n^2)
     *
     * 数学公式：
     * x[n-1] = b[n-1] / A[n-1][n-1]
     * x[i] = (b[i] - Σ(j=i+1 to n-1) A[i][j] * x[j]) / A[i][i]
     */
    void back_substitution(const Matrix& A, const Vector& b, Vector& x);
};

#endif // GAUSSIAN_SOLVER_H
