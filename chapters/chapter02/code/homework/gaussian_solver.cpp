/**
 * @file gaussian_solver.cpp
 * @brief 高斯消元求解器的实现文件
 *
 * 包含高斯消元算法的完整实现，演示：
 * - 前向消元过程
 * - 部分选主元策略
 * - 回代求解过程
 * - 数值稳定性处理
 * - 错误检测和处理
 */

#include "gaussian_solver.h"
#include <algorithm>    // 用于std::swap等算法
#include <cmath>        // 用于数学函数

/**
 * @brief 高斯消元求解器主函数
 *
 * @param A_orig 原始系数矩阵
 * @param b_orig 原始常数向量
 * @return 解向量，如果求解失败返回nullptr
 *
 * 算法流程：
 * 1. 验证输入参数（方阵检查、奇异性检查）
 * 2. 复制数据避免修改原始输入
 * 3. 执行前向消元（转换为上三角矩阵）
 * 4. 执行回代求解（得到最终解）
 * 5. 返回解向量
 *
 * 设计原则：
 * - 避免修改输入参数
 * - 早期检测错误条件
 * - 模块化设计便于测试和维护
 */
std::unique_ptr<Vector> GaussianSolver::solve(const Matrix& A_orig, const Vector& b_orig) {

    // 奇异性检查：奇异矩阵无法求解
    if (A_orig.is_singular()) {
        return nullptr; // 矩阵奇异，无法使用高斯消元求解
    }

    // 复制矩阵和向量以避免修改原数据（const正确性）
    Matrix A = A_orig;  // 复制系数矩阵
    Vector b = b_orig;  // 复制常数向量
    size_t n = A.rows();  // 获取矩阵大小

    // 执行前向消元：将矩阵转换为上三角形式
    if (!forward_elimination(A, b)) {
        return nullptr; // 前向消元失败（通常是奇异矩阵）
    }

    // 回代求解：从上三角矩阵得到解向量
    auto x = std::make_unique<Vector>(n);  // 创建解向量
    back_substitution(A, b, *x);           // 执行回代

    return x;  // 返回解向量
}

/**
 * @brief 前向消元过程
 *
 * @param A 系数矩阵（会被修改）
 * @param b 常数向量（会被修改）
 * @return 如果消元成功返回true，失败返回false
 *
 * 算法：高斯消元的前向消元阶段
 * 目的：将系数矩阵转换为上三角矩阵
 *
 * 主要步骤：
 * 1. 部分选主元：选择数值最大的元素作为主元
 * 2. 行交换：将主元交换到对角线位置
 * 3. 消元：使用主元消去下方元素
 *
 * 数值稳定性：
 * - 选主元：避免除以小数导致的误差放大
 * - 阈值1e-10：检测零主元（奇异矩阵）
 * - 行交换：保持数值稳定性
 *
 * 时间复杂度：O(n^3)
 * 空间复杂度：O(1)（原地操作）
 */
bool GaussianSolver::forward_elimination(Matrix& A, Vector& b) {
    const size_t n = A.rows();

    auto& A_data = A.get_mutable_raw_data(); // 优化：获取可修改的底层数据引用

    for (size_t i = 0; i < n - 1; ++i) {
        // ===== 部分选主元策略 =====
        // 使用std::max_element和Lambda表达式优化主元查找
        auto max_iter = std::max_element(
            A_data.begin() + i, A_data.end(),
            [i](const std::vector<double>& row1, const std::vector<double>& row2) {
                return std::abs(row1[i]) < std::abs(row2[i]);
            }
        );
        size_t max_row = std::distance(A_data.begin(), max_iter);
        double max_val = std::abs((*max_iter)[i]);

        // 检查主元是否接近零
        if (max_val < 1e-10) {
            return false;  // 奇异矩阵，无法继续消元
        }

        // 如果需要，交换行以获得最大主元
        if (max_row != i) {
            // 优化：直接交换底层vector，避免多次调用at
            std::swap_ranges(
                A_data[i].begin(), A_data[i].end(),
                A_data[max_row].begin()
            );
            std::swap(b.at(i), b.at(max_row));
        }

        // ===== 消元过程 =====
        // 对第i列下方的所有行进行消元
        // 优化：消元时只更新必要的元素，避免不必要的循环
        const double pivot = A.at(i, i);
        for (size_t k = i + 1; k < n; ++k) {
            double factor = A.at(k, i) / pivot;
            // 优化：利用指针批量操作
            auto row_k = A_data[k].begin();
            auto row_i = A_data[i].begin();
            for (size_t j = i; j < n; ++j) {
                row_k[j] -= factor * row_i[j];
            }
            b.at(k) -= factor * b.at(i);
        }
    }

    return true;  // 消元成功完成
}

/**
 * @brief 回代求解过程
 *
 * @param A 上三角系数矩阵
 * @param b 常数向量
 * @param x 解向量（输出参数）
 *
 * 算法：从上三角线性方程组求解
 * 原理：从最后一个方程开始向前求解
 *
 * 数学过程：
 * 1. 从第n-1个方程开始：x[n-1] = b[n-1] / A[n-1][n-1]
 * 2. 对于第i个方程：x[i] = (b[i] - Σ(j=i+1 to n-1) A[i][j]*x[j]) / A[i][i]
 * 3. 继续向前直到第一个方程
 *
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(1)
 *
 * 数值稳定性：
 * - 从后向前求解，避免了前向依赖
 * - 每个变量只依赖于前面已求解的变量
 * - 保证了解的唯一性
 *
 * 演示：
 * - 逆向循环的使用
 * - 累减操作的实现
 * - 数值计算的精度控制
 */
void GaussianSolver::back_substitution(const Matrix& A, const Vector& b, Vector& x) {
    size_t n = A.rows();

    // 从最后一个方程开始向前求解
    for (int i = n - 1; i >= 0; --i) {
        // 开始时将常数项赋值给x[i]
        x.at(i) = b.at(i);

        // 减去已求解变量的影响
        for (size_t j = i + 1; j < n; ++j) {
            x.at(i) -= A.at(i, j) * x.at(j);
        }

        // 除以主对角元素得到最终解
        x.at(i) /= A.at(i, i);
    }
}
