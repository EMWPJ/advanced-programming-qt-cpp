#include "linear_system.h"
#include <iostream>
#include <cmath>  // 用于数学计算
#include <algorithm>    // 用于算法函数
#include <numeric>      // 用于数值算法

LinearSystem::LinearSystem(const Matrix& A, const Vector& b) : m_A(A), m_b(b) {}

std::unique_ptr<Vector> LinearSystem::solve(std::unique_ptr<SolverBase> solver) {
    if (!solver) {
        return nullptr; // Solver cannot be null
    }

    // 执行求解并自动保存数值解
    auto solution = solver->solve(m_A, m_b);
    if (solution) {
        m_numerical_solution = std::make_unique<Vector>(*solution);
    }

    return solution;
}

void LinearSystem::print() const {
    std::cout << "系数矩阵A：" << std::endl;
    m_A.print();
    std::cout << "常数向量b：" << std::endl;
    m_b.print();
}

// ===== 解的管理方法 =====

void LinearSystem::set_exact_solution(std::unique_ptr<Vector> exact_solution) {
    m_exact_solution = std::move(exact_solution);
}

void LinearSystem::set_numerical_solution(std::unique_ptr<Vector> numerical_solution) {
    m_numerical_solution = std::move(numerical_solution);
}

const Vector* LinearSystem::get_exact_solution() const {
    return m_exact_solution.get();
}

const Vector* LinearSystem::get_numerical_solution() const {
    return m_numerical_solution.get();
}

// ===== 精度验证方法 =====

double LinearSystem::compute_residual(const Vector& x) const {
    // 优化：直接调用Matrix的multiply方法进行矩阵-向量乘法
    auto Ax_ptr = m_A.multiply(x.get_raw_data()); // 返回unique_ptr<std::vector<double>>
    const auto& Ax = *Ax_ptr;

    // 优化：使用std::transform一次性计算残差向量 residual = b - Ax
    std::vector<double> residual_data(m_b.size());
    const auto& b_data = m_b.get_raw_data();
    std::transform(b_data.begin(), b_data.end(), Ax.begin(), residual_data.begin(),
                   [](double b_i, double ax_i) { return b_i - ax_i; });

    // 直接计算范数，无需额外构造Vector对象
    double sum_of_squares = std::accumulate(residual_data.begin(), residual_data.end(), 0.0,
                                            [](double acc, double val) { return acc + val * val; });
    return std::sqrt(sum_of_squares);
}

double LinearSystem::compute_error() const {
    if (!m_exact_solution || !m_numerical_solution) {
        return -1.0; // 未设置解析解或数值解
    }

    std::vector<double> error_data(m_numerical_solution->size());

    // 使用std::transform和Lambda表达式优化误差计算：error[i] = numerical[i] - exact[i]
    auto num_data = m_numerical_solution->get_raw_data();
    auto exact_data = m_exact_solution->get_raw_data();
    std::transform(num_data.begin(), num_data.end(), exact_data.begin(), error_data.begin(),
                   [](double num, double exact) { return num - exact; });

    Vector error(error_data);
    return error.norm();
}

bool LinearSystem::validate_solution(const Vector& x, double tolerance) const {
    double residual = compute_residual(x);

    // 同时考虑绝对误差和相对误差
    double b_norm = m_b.norm();
    double relative_residual = (b_norm > 1e-12) ? residual / b_norm : residual;

    // 绝对误差检查
    bool absolute_ok = (residual < tolerance);

    // 相对误差检查（更严格的标准）
    bool relative_ok = (relative_residual < tolerance * 10.0);

    return absolute_ok && relative_ok;
}

void LinearSystem::print_accuracy_report() const {
    std::cout << "\n===== 精度分析报告 =====" << std::endl;

    // 残差分析
    if (m_numerical_solution) {
        double residual = compute_residual(*m_numerical_solution);
        std::cout << "残差分析：" << std::endl;
        std::cout << "  残差范数 ||Ax - b|| = " << residual << std::endl;

        if (residual < 1e-10) {
            std::cout << "  状态：非常精确" << std::endl;
        } else if (residual < 1e-6) {
            std::cout << "  状态：数值精确" << std::endl;
        } else if (residual < 1e-3) {
            std::cout << "  状态：基本精确" << std::endl;
        } else {
            std::cout << "  状态：可能有较大误差" << std::endl;
        }
    }

    // 误差分析
    if (m_exact_solution && m_numerical_solution) {
        double error = compute_error();
        std::cout << "\n误差分析：" << std::endl;
        std::cout << "  与精确解的误差 ||x_num - x_exact|| = " << error << std::endl;

        if (error < 1e-10) {
            std::cout << "  状态：与精确解高度一致" << std::endl;
        } else if (error < 1e-6) {
            std::cout << "  状态：与精确解基本一致" << std::endl;
        } else {
            std::cout << "  状态：与精确解存在偏差" << std::endl;
        }
    }

    // 数值稳定性分析
    std::cout << "\n数值稳定性分析：" << std::endl;
    std::cout << "  矩阵类型: " << (m_A.rows() == m_A.cols() ? "方阵" : "非方阵") << std::endl;

    // 矩阵特征分析
    std::cout << "  矩阵维度: " << m_A.rows() << " x " << m_A.cols() << std::endl;
    std::cout << "  向量维度: " << m_b.size() << std::endl;

    std::cout << "===== 报告完成 =====\n" << std::endl;
}
