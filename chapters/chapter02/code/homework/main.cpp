/**
 * @file main.cpp
 * @brief 线性方程组求解系统 - 数值算法综合演示
 *
 * 程序特性：
 * - 面向对象设计：Matrix、Vector、SolverBase、LinearSystem类
 * - 多种求解算法：高斯消元、LU分解、雅可比迭代、SOR迭代
 * - 智能指针管理：std::unique_ptr自动内存管理
 * - 数值验证：残差计算、误差分析、解的验证
 * - 对角占优检测：迭代法收敛性分析
 * - UTF-8编码支持：中文界面显示
 *
 * 编译要求：
 * - C++17标准
 * - CMake构建系统
 * - UTF-8编码支持
 */

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <windows.h>

#include "matrix.h"
#include "vector.h"
#include "solver_base.h"
#include "gaussian_solver.h"
#include "lu_solver.h"
#include "jacobi_solver.h"
#include "sor_solver.h"
#include "linear_system.h"

/**
 * @brief 主函数 - 线性方程组求解系统演示
 * @return int 程序退出状态
 *
 * 程序流程：
 * 1. 设置UTF-8编码
 * 2. 创建测试矩阵（5阶对角占优系统）
 * 3. 创建各种求解器
 * 4. 逐个测试求解器
 * 5. 验证求解结果
 * 6. 生成精度报告
 */
int main() {
    // ===== 设置编码 =====
    SetConsoleOutputCP(65001);

    // 设置输出精度
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "矩阵分析：" << std::endl;
    std::cout << "系数矩阵A：" << std::endl;

    // ===== 创建测试矩阵（5阶对角占优系统） =====
    std::vector<std::vector<double>> A_data = {
        {5, 1, 0, 1, 0},
        {1, 4, 1, 0, 0},
        {0, 1, 5, 0, 1},
        {0, 0, 1, 4, 0},
        {1, 0, 0, 1, 5}
    };

    std::vector<double> b_data = {8, 7, 9, 6, 7};

    Matrix A(A_data);
    Vector b(b_data);

    // 创建线性方程组对象
    LinearSystem system(A, b);

    // 打印矩阵
    A.print();

    // ===== 矩阵奇异性分析 =====
    bool singular = A.is_singular();
    std::cout << "矩阵奇异性判断结果: " << (singular ? "奇异（不可逆）" : "非奇异（可逆）") << std::endl;

    // ===== 计算精确解作为参考 =====

    /**
     * 使用LU分解计算精确解作为参考
     * 后续用于验证其他算法的精度
     */
    std::unique_ptr<SolverBase> exact_solver = std::make_unique<LUSolver>();
    std::unique_ptr<Vector> exact_solution = system.solve(std::move(exact_solver));

    // 设置解析解用于精度验证
    if (exact_solution) {
        system.set_exact_solution(std::make_unique<Vector>(*exact_solution));
        std::cout << "精确解（使用LU分解）：" << std::endl;
        exact_solution->print();
    }

    // 创建具体的求解器
    auto gaussian_solver = std::make_unique<GaussianSolver>();
    auto lu_solver = std::make_unique<LUSolver>();
    auto jacobi_solver = std::make_unique<JacobiSolver>(1e-6, 1000);
    auto sor_solver = std::make_unique<SORSolver>(1.2, 1e-6, 1000);

    // 测试高斯消元
    std::unique_ptr<Vector> x_gaussian = system.solve(std::move(gaussian_solver));
    if (x_gaussian) {
        std::cout << "使用高斯消元求解结果：" << std::endl;
        x_gaussian->print();
        double residual_gaussian = system.compute_residual(*x_gaussian);
        std::cout << "残差范数 ||Ax - b||: " << residual_gaussian << std::endl;

        // 计算与精确解的误差
        const Vector* exact = system.get_exact_solution();
        if (exact) {
            std::vector<double> error_data(x_gaussian->size());
            auto gauss_data = x_gaussian->get_raw_data();
            auto exact_data = exact->get_raw_data();
            for (size_t i = 0; i < x_gaussian->size(); ++i) {
                error_data[i] = gauss_data[i] - exact_data[i];
            }
            Vector error(error_data);
            double error_gaussian = error.norm();
            std::cout << "与精确解的误差 ||x - x_exact||: " << error_gaussian << std::endl;
        }

        bool is_accurate_gaussian = system.validate_solution(*x_gaussian);
        std::cout << "解的准确性验证: " << (is_accurate_gaussian ? "通过" : "未通过") << std::endl;
        system.print_accuracy_report();
        std::cout << std::endl;
    } else {
        std::cout << "使用高斯消元求解失败" << std::endl;
    }

    // 测试LU分解
    std::unique_ptr<Vector> x_lu = system.solve(std::move(lu_solver));
    if (x_lu) {
        std::cout << "使用LU分解求解结果：" << std::endl;
        x_lu->print();
        double residual_lu = system.compute_residual(*x_lu);
        std::cout << "残差范数 ||Ax - b||: " << residual_lu << std::endl;
        double error_lu = system.compute_error();
        if (error_lu >= 0.0) {
            std::cout << "与精确解的误差 ||x - x_exact||: " << error_lu << std::endl;
        }
        bool is_accurate_lu = system.validate_solution(*x_lu);
        std::cout << "解的准确性验证: " << (is_accurate_lu ? "通过" : "未通过") << std::endl;
        system.print_accuracy_report();
        std::cout << std::endl;
    } else {
        std::cout << "使用LU分解求解失败" << std::endl;
    }

    // 测试雅可比迭代
    std::unique_ptr<Vector> x_jacobi = system.solve(std::move(jacobi_solver));
    if (x_jacobi) {
        std::cout << "使用雅可比迭代求解结果：" << std::endl;
        x_jacobi->print();
        double residual_jacobi = system.compute_residual(*x_jacobi);
        std::cout << "残差范数 ||Ax - b||: " << residual_jacobi << std::endl;
        double error_jacobi = system.compute_error();
        if (error_jacobi >= 0.0) {
            std::cout << "与精确解的误差 ||x - x_exact||: " << error_jacobi << std::endl;
        }
        bool is_accurate_jacobi = system.validate_solution(*x_jacobi);
        std::cout << "解的准确性验证: " << (is_accurate_jacobi ? "通过" : "未通过") << std::endl;
        system.print_accuracy_report();
        std::cout << std::endl;
    } else {
        std::cout << "使用雅可比迭代求解失败" << std::endl;
    }

    // 测试SOR迭代
    std::unique_ptr<Vector> x_sor = system.solve(std::move(sor_solver));
    if (x_sor) {
        std::cout << "使用SOR迭代求解结果：" << std::endl;
        x_sor->print();
        double residual_sor = system.compute_residual(*x_sor);
        std::cout << "残差范数 ||Ax - b||: " << residual_sor << std::endl;
        double error_sor = system.compute_error();
        if (error_sor >= 0.0) {
            std::cout << "与精确解的误差 ||x - x_exact||: " << error_sor << std::endl;
        }
        bool is_accurate_sor = system.validate_solution(*x_sor);
        std::cout << "解的准确性验证: " << (is_accurate_sor ? "通过" : "未通过") << std::endl;
        system.print_accuracy_report();
        std::cout << std::endl;
    } else {
        std::cout << "使用SOR迭代求解失败" << std::endl;
    }

    // ===== 程序完成 =====

    /**
     * 程序总结：
     * - 展示了C++面向对象设计的强大功能
     * - 实现了多种数值算法的工程实现
     * - 体现了C++11/14现代特性的使用
     * - 展示了软件工程最佳实践
     */
    std::cout << std::endl << "程序执行完成！" << std::endl;
    std::cout << std::flush;  // 确保输出立即显示

    // ===== 恢复原始编码设置 =====
    // 程序结束时编码会自动恢复，无需手动处理

    return 0;
}