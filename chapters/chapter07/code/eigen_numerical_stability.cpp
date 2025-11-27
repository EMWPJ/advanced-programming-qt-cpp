#include <iostream>
#include <chrono>
#include <limits>
#include <Eigen/Dense>
#include <cmath>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;

// 数值稳定性示例：展示如何检查和处理数值问题

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen数值稳定性示例 ===" << endl;

    // 示例1: 检查矩阵条件数
    cout << "\n--- 示例1: 检查矩阵条件数 ---" << endl;

    // 创建一个病态矩阵（条件数很大）
    MatrixXd ill_conditioned(3, 3);
    ill_conditioned << 1.0, 2.0, 3.0,
                       2.0, 4.0001, 6.0,
                       3.0, 6.0, 9.0;

    // 计算条件数
    JacobiSVD<MatrixXd> svd(ill_conditioned);
    VectorXd singular_values = svd.singularValues();
    double condition_number = singular_values(0) /
                              singular_values(singular_values.size() - 1);

    cout << "矩阵:" << endl << ill_conditioned << endl;
    cout << "条件数: " << condition_number << endl;

    if (condition_number > 1e12) {
        cout << "警告: 矩阵病态，条件数过大！" << endl;
        cout << "建议: 使用正则化或特殊求解方法" << endl;
    } else if (condition_number > 1e8) {
        cout << "注意: 矩阵条件数较大，可能影响精度" << endl;
    } else {
        cout << "矩阵条件良好" << endl;
    }

    // 示例2: 避免直接求逆
    cout << "\n--- 示例2: 避免直接求逆 ---" << endl;

    MatrixXd A = MatrixXd::Random(100, 100);
    A = A.transpose() * A;  // 对称正定矩阵
    VectorXd b = VectorXd::Random(100);

    // 不好的做法：直接求逆
    auto start = chrono::high_resolution_clock::now();
    VectorXd x1 = A.inverse() * b;
    auto end = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<microseconds>(end - start);
    cout << "直接求逆耗时: " << duration1.count() << " 微秒" << endl;

    // 好的做法：使用分解
    start = chrono::high_resolution_clock::now();
    VectorXd x2 = A.llt().solve(b);  // Cholesky分解
    end = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<microseconds>(end - start);
    cout << "Cholesky分解耗时: " << duration2.count() << " 微秒" << endl;
    cout << "性能提升: " << (double)duration1.count() / duration2.count()
         << "x" << endl;

    // 示例3: 验证解的正确性
    cout << "\n--- 示例3: 验证解的正确性 ---" << endl;

    VectorXd residual = A * x2 - b;
    double residual_norm = residual.norm();
    double relative_error = residual_norm / b.norm();

    cout << "残差范数: " << residual_norm << endl;
    cout << "相对误差: " << relative_error << endl;

    if (relative_error < 1e-10) {
        cout << "解非常准确" << endl;
    } else if (relative_error < 1e-6) {
        cout << "解精度良好" << endl;
    } else {
        cout << "警告: 解精度可能不足，需要改进算法" << endl;
    }

    // 示例4: 检查NaN和Inf
    cout << "\n--- 示例4: 检查NaN和Inf ---" << endl;

    MatrixXd test_matrix(3, 3);
    test_matrix << 1.0, 2.0, 3.0,
                   0.0, 0.0, 0.0,  // 奇异矩阵
                   4.0, 5.0, 6.0;

    MatrixXd inv_matrix;
    try {
        inv_matrix = test_matrix.inverse();

        // 检查结果
        bool has_nan = !((inv_matrix.array() == inv_matrix.array()).all());
        bool has_inf = ((inv_matrix.array().abs() == std::numeric_limits<double>::infinity()).any());
        if (has_nan) {
            cout << "警告: 结果包含NaN值！" << endl;
        }
        if (has_inf) {
            cout << "警告: 结果包含Inf值！" << endl;
        }

        cout << "逆矩阵:" << endl << inv_matrix << endl;
    } catch (...) {
        cout << "错误: 矩阵不可逆" << endl;
    }

    // 更好的做法：使用分解并检查
    PartialPivLU<MatrixXd> lu(test_matrix);
    if (lu.determinant() == 0.0) {
        cout << "矩阵奇异，无法求解" << endl;
    } else {
        VectorXd test_b(3);
        test_b << 1.0, 2.0, 3.0;
        VectorXd test_x = lu.solve(test_b);
        cout << "使用LU分解求解成功" << endl;
        cout << "解向量:" << endl << test_x << endl;
    }

    // 示例5: 使用QR分解处理超定系统
    cout << "\n--- 示例5: 超定系统的最小二乘解 ---" << endl;

    MatrixXd overdetermined = MatrixXd::Random(50, 10);
    VectorXd overdetermined_b = VectorXd::Random(50);

    // 使用QR分解求解最小二乘问题
    VectorXd x_ls = overdetermined.householderQr().solve(overdetermined_b);

    VectorXd residual_ls = overdetermined * x_ls - overdetermined_b;
    double residual_norm_ls = residual_ls.norm();

    cout << "残差范数: " << residual_norm_ls << endl;
    cout << "这是最小二乘意义下的最优解" << endl;

    cout << "\n=== 数值稳定性总结 ===" << endl;
    cout << "1. 检查矩阵条件数，识别病态矩阵" << endl;
    cout << "2. 避免直接求逆，使用分解方法" << endl;
    cout << "3. 计算残差验证解的正确性" << endl;
    cout << "4. 检查NaN和Inf值" << endl;
    cout << "5. 使用相对误差评估精度" << endl;
    cout << "6. 对病态问题使用正则化或特殊算法" << endl;

    cin.get();
    return 0;
}
