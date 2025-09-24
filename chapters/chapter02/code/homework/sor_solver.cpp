#include "sor_solver.h"
#include <cmath>
#include <algorithm>    // 用于算法函数
#include <numeric>      // 用于数值算法

SORSolver::SORSolver(double omega, double tolerance, size_t max_iterations)
    : m_omega(omega), m_tolerance(tolerance), m_max_iterations(max_iterations) {}

std::unique_ptr<Vector> SORSolver::solve(const Matrix& A, const Vector& b) {
    size_t n = A.rows();
    auto x = std::make_unique<Vector>(n, 0.0);  // 初始解

    // 预先获取底层数据指针，减少函数调用开销
    const auto& A_data = A.get_raw_data();
    const auto& b_data = b.get_raw_data();

    std::vector<double> x_vec(n, 0.0);
    std::vector<double> x_new_vec(n, 0.0);

    for (size_t iter = 0; iter < m_max_iterations; ++iter) {
        // 直接在vector上操作，提升效率
        std::copy(x_vec.begin(), x_vec.end(), x_new_vec.begin());

        for (size_t i = 0; i < n; ++i) {
            // 使用std::inner_product和Lambda表达式优化sum1和sum2计算
            double sum1 = std::inner_product(
                A_data[i].begin(), A_data[i].begin() + i,
                x_new_vec.begin(), 0.0
            );
            double sum2 = std::inner_product(
                A_data[i].begin() + i + 1, A_data[i].end(),
                x_vec.begin() + i + 1, 0.0
            );

            if (std::abs(A_data[i][i]) < 1e-10) {
                return nullptr; // 矩阵对角元素为0，无法使用SOR方法
            }

            x_new_vec[i] = (1 - m_omega) * x_vec[i] + m_omega * (b_data[i] - sum1 - sum2) / A_data[i][i];
        }

        // 使用std::inner_product和Lambda表达式优化误差计算
        double error = std::sqrt(std::inner_product(
            x_new_vec.begin(), x_new_vec.end(), x_vec.begin(), 0.0,
            std::plus<>(),
            [](double a, double b) { return (a - b) * (a - b); }
        ));

        if (iter < 5) { // 只显示前5次迭代
            std::cout << "SOR迭代 " << iter << ", 误差 = " << error << ", 松弛因子 = " << m_omega << std::endl;
        }

        if (error < m_tolerance) {
            return std::make_unique<Vector>(x_new_vec);
        }

        std::swap(x_vec, x_new_vec);  // 更新解
    }

    return nullptr; // SOR方法未在最大迭代次数内收敛
}
