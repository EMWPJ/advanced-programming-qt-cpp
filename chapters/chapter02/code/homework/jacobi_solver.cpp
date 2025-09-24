#include "jacobi_solver.h"
#include <cmath>
#include <algorithm>    // 用于算法函数
#include <numeric>      // 用于数值算法

JacobiSolver::JacobiSolver(double tolerance, size_t max_iterations)
    : m_tolerance(tolerance), m_max_iterations(max_iterations) {}

std::unique_ptr<Vector> JacobiSolver::solve(const Matrix& A, const Vector& b) {

    size_t n = A.rows();
    auto x = std::make_unique<Vector>(n, 0.0);  // 初始解
    auto x_new = std::make_unique<Vector>(n);

    for (size_t iter = 0; iter < m_max_iterations; ++iter) {
        // 计算新解
        for (size_t i = 0; i < n; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < n; ++j) {
                if (j != i) {
                    sum += A.at(i, j) * x->at(j);
                }
            }
            if (std::abs(A.at(i, i)) < 1e-10) {
                return nullptr; // 矩阵对角元素为0，无法使用雅可比迭代法
            }
            x_new->at(i) = (b.at(i) - sum) / A.at(i, i);
        }

        // 检查收敛性 - 使用传统循环实现
        double error = 0.0;
        auto x_new_data = x_new->get_raw_data();
        auto x_data = x->get_raw_data();
        for (size_t i = 0; i < x->size(); ++i) {
            double diff = x_new_data[i] - x_data[i];
            error += diff * diff;
        }
        error = std::sqrt(error);

        if (iter < 5) { // 只显示前5次迭代
            std::cout << "雅可比迭代 " << iter << ", 误差 = " << error << std::endl;
        }

        if (error < m_tolerance) {
            return x_new;
        }

        // 交换指针
        std::swap(x, x_new);
    }

    return nullptr; //雅可比迭代法未在最大迭代次数内收敛
}
