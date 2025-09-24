/**
 * @file lu_solver.h
 * @brief LU分解求解器 - 演示分解求解方法
 *
 * 这个求解器展示了：
 * - LU分解算法的实现
 * - 分解求解方法的基本原理
 * - Doolittle分解（L的下三角，U的上三角）
 * - 前向和后向替换求解
 * - 数值稳定性分析
 */

#ifndef LU_SOLVER_H
#define LU_SOLVER_H

#include "solver_base.h"  // 基类定义
#include "matrix.h"       // 矩阵类
#include "vector.h"       // 向量类

/**
 * @class LUSolver
 * @brief LU分解求解器类
 *
 * 算法描述：
 * LU分解法将系数矩阵A分解为下三角矩阵L和上三角矩阵U的乘积
 * 主要步骤：
 * 1. LU分解：A = L * U
 * 2. 前向替换：L * y = b 求解y
 * 3. 后向替换：U * x = y 求解x
 *
 * 数学原理：
 * - Doolittle分解：L的单位下三角，U的上三角
 * - 分解唯一性：对于非奇异矩阵
 * - 数值稳定性：优于高斯消元
 *
 * 适用性：
 * - 适用于方阵
 * - 适合多次求解同一矩阵不同右端项的问题
 * - 时间复杂度：O(n^3)
 * - 空间复杂度：O(n^2)
 *
 * 优势：
 * - 避免了高斯消元中的除法操作
 * - 更适合并行计算
 * - 易于扩展到其他分解方法
 *
 * 演示：
 * - 分解求解的模块化设计
 * - 内存管理的优化
 * - 数值算法的工程实现
 */
class LUSolver : public SolverBase {
public:
    /**
     * @brief 求解线性方程组
     *
     * @param A 系数矩阵
     * @param b 常数向量
     * @return 解向量，如果失败返回nullptr
     *
     * 算法流程：
     * 1. 验证输入参数（方阵检查）
     * 2. 执行LU分解
     * 3. 如果分解成功，执行前向和后向替换
     * 4. 返回解向量
     *
     * 设计特点：
     * - 使用原地分解节省内存
     * - 分离分解和求解过程
     * - 便于多次求解同一矩阵
     */
    std::unique_ptr<Vector> solve(const Matrix& A, const Vector& b) override;


private:
    /**
     * @brief LU分解过程（Doolittle方法）
     *
     * @param A 输入系数矩阵
     * @param L 下三角矩阵（输出）
     * @param U 上三角矩阵（输出）
     * @return 如果分解成功返回true
     *
     * 算法：
     * - L的单位下三角：对角线元素为1
     * - U的上三角：包含主要的数值
     * - 部分选主元提高稳定性
     *
     * 数学公式：
     * L[i][j] = (A[i][j] - Σ(k=0 to j-1) L[i][k]*U[k][j]) / U[j][j]  (i > j)
     * U[i][j] = A[i][j] - Σ(k=0 to i-1) L[i][k]*U[k][j]  (i ≤ j)
     *
     * 数值考虑：
     * - 避免除零错误
     * - 检测奇异矩阵
     * - 控制舍入误差
     */
    bool lu_decomposition(const Matrix& A, Matrix& L, Matrix& U);

    /**
     * @brief 前向替换求解
     *
     * @param L 下三角矩阵
     * @param b 常数向量
     * @param y 解向量（输出）
     *
     * 算法：
     * - 从第一个方程开始向前求解
     * - 每个变量依赖于前面已求解的变量
     * - 时间复杂度：O(n^2)
     *
     * 数学公式：
     * y[0] = b[0] / L[0][0]
     * y[i] = (b[i] - Σ(j=0 to i-1) L[i][j]*y[j]) / L[i][i]
     */
    void forward_substitution(const Matrix& L, const Vector& b, Vector& y);

    /**
     * @brief 后向替换求解
     *
     * @param U 上三角矩阵
     * @param y 中间向量
     * @param x 最终解向量（输出）
     *
     * 算法：
     * - 从最后一个方程开始向后求解
     * - 每个变量依赖于后面已求解的变量
     * - 时间复杂度：O(n^2)
     *
     * 数学公式：
     * x[n-1] = y[n-1] / U[n-1][n-1]
     * x[i] = (y[i] - Σ(j=i+1 to n-1) U[i][j]*x[j]) / U[i][i]
     */
    void back_substitution(const Matrix& U, const Vector& y, Vector& x);
};

#endif // LU_SOLVER_H
