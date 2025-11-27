#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;

// Eigen矩阵分解方法对比示例

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵分解方法对比 ===" << endl;

    // ========== 1. 不同分解方法的基本使用 ==========
    cout << "\n--- 1. 不同分解方法的基本使用 ---" << endl;

    MatrixXd A(4, 4);
    A << 4, 1, 0, 0,
         1, 4, 1, 0,
         0, 1, 4, 1,
         0, 0, 1, 4;

    VectorXd b(4);
    b << 1, 2, 3, 4;

    cout << "系数矩阵A:\n" << A << endl;
    cout << "右端向量b:\n" << b << endl;

    // LU分解
    cout << "\n--- LU分解 ---" << endl;
    PartialPivLU<MatrixXd> lu(A);
    VectorXd x_lu = lu.solve(b);
    cout << "LU分解解:\n" << x_lu << endl;
    VectorXd residual_lu = A * x_lu - b;
    cout << "残差:\n" << residual_lu << endl;
    cout << "残差范数: " << residual_lu.norm() << endl;

    // QR分解
    cout << "\n--- QR分解 ---" << endl;
    HouseholderQR<MatrixXd> qr(A);
    VectorXd x_qr = qr.solve(b);
    cout << "QR分解解:\n" << x_qr << endl;
    VectorXd residual_qr = A * x_qr - b;
    cout << "残差范数: " << residual_qr.norm() << endl;

    // Cholesky分解（对称正定）
    cout << "\n--- Cholesky分解（对称正定矩阵）---" << endl;
    MatrixXd sym_A = A.transpose() * A;  // 构造对称正定矩阵
    VectorXd sym_b = sym_A * VectorXd::Ones(4);

    LLT<MatrixXd> chol(sym_A);
    if (chol.info() == Success) {
        VectorXd x_chol = chol.solve(sym_b);
        cout << "Cholesky分解解:\n" << x_chol << endl;
        VectorXd residual_chol = sym_A * x_chol - sym_b;
        cout << "残差范数: " << residual_chol.norm() << endl;
    } else {
        cout << "Cholesky分解失败（矩阵不是正定）" << endl;
    }

    // SVD分解
    cout << "\n--- SVD分解 ---" << endl;
    JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
    VectorXd x_svd = svd.solve(b);
    cout << "SVD分解解:\n" << x_svd << endl;
    VectorXd residual_svd = A * x_svd - b;
    cout << "残差范数: " << residual_svd.norm() << endl;

    // 显示奇异值
    VectorXd singular_values = svd.singularValues();
    cout << "奇异值:\n" << singular_values << endl;
    double condition_number = singular_values(0) /
                              singular_values(singular_values.size() - 1);
    cout << "条件数: " << condition_number << endl;

    // ========== 2. 超定系统：最小二乘 ==========
    cout << "\n--- 2. 超定系统：最小二乘 ---" << endl;

    MatrixXd A_over(6, 3);
    A_over << 1, 1, 1,
              1, 2, 4,
              1, 3, 9,
              1, 4, 16,
              1, 5, 25,
              1, 6, 36;

    VectorXd b_over(6);
    b_over << 1, 4, 9, 16, 25, 36;

    cout << "超定系统 A (6x3):\n" << A_over << endl;
    cout << "右端向量 b:\n" << b_over << endl;

    // QR分解求解最小二乘
    HouseholderQR<MatrixXd> qr_over(A_over);
    VectorXd x_ls = qr_over.solve(b_over);
    cout << "最小二乘解:\n" << x_ls << endl;

    VectorXd residual_ls = A_over * x_ls - b_over;
    cout << "残差:\n" << residual_ls << endl;
    cout << "残差平方和: " << residual_ls.squaredNorm() << endl;

    // ========== 3. 病态矩阵处理 ==========
    cout << "\n--- 3. 病态矩阵处理 ---" << endl;

    // 创建病态矩阵（Hilbert矩阵）
    MatrixXd hilbert(4, 4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            hilbert(i, j) = 1.0 / (i + j + 1.0);
        }
    }

    VectorXd b_hilbert(4);
    b_hilbert << 1, 1, 1, 1;

    cout << "Hilbert矩阵（病态）:\n" << hilbert << endl;

    // 计算条件数
    JacobiSVD<MatrixXd> svd_hilbert(hilbert, ComputeThinU | ComputeThinV);
    VectorXd sv_hilbert = svd_hilbert.singularValues();
    double cond_hilbert = sv_hilbert(0) / sv_hilbert(sv_hilbert.size() - 1);
    cout << "条件数: " << cond_hilbert << " (很大，矩阵病态)" << endl;

    // 使用SVD求解（最稳定）
    VectorXd x_hilbert_svd = svd_hilbert.solve(b_hilbert);
    cout << "SVD求解:\n" << x_hilbert_svd << endl;

    // 使用LU求解（可能不稳定）
    PartialPivLU<MatrixXd> lu_hilbert(hilbert);
    VectorXd x_hilbert_lu = lu_hilbert.solve(b_hilbert);
    cout << "LU求解:\n" << x_hilbert_lu << endl;

    // 比较残差
    VectorXd residual_svd_h = hilbert * x_hilbert_svd - b_hilbert;
    VectorXd residual_lu_h = hilbert * x_hilbert_lu - b_hilbert;
    cout << "SVD残差范数: " << residual_svd_h.norm() << endl;
    cout << "LU残差范数: " << residual_lu_h.norm() << endl;

    // ========== 4. 性能对比 ==========
    cout << "\n--- 4. 性能对比 ---" << endl;

    const int size = 200;
    MatrixXd A_large = MatrixXd::Random(size, size);
    A_large = A_large.transpose() * A_large;  // 对称正定
    VectorXd b_large = VectorXd::Random(size);

    const int iterations = 10;

    // LU分解性能
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        PartialPivLU<MatrixXd> lu_perf(A_large);
        VectorXd x = lu_perf.solve(b_large);
    }
    auto end = high_resolution_clock::now();
    auto duration_lu = duration_cast<microseconds>(end - start);
    cout << "LU分解平均耗时: " << duration_lu.count() / iterations << " 微秒" << endl;

    // QR分解性能
    start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        HouseholderQR<MatrixXd> qr_perf(A_large);
        VectorXd x = qr_perf.solve(b_large);
    }
    end = high_resolution_clock::now();
    auto duration_qr = duration_cast<microseconds>(end - start);
    cout << "QR分解平均耗时: " << duration_qr.count() / iterations << " 微秒" << endl;

    // Cholesky分解性能（最快）
    start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        LLT<MatrixXd> chol_perf(A_large);
        VectorXd x = chol_perf.solve(b_large);
    }
    end = high_resolution_clock::now();
    auto duration_chol = duration_cast<microseconds>(end - start);
    cout << "Cholesky分解平均耗时: " << duration_chol.count() / iterations << " 微秒" << endl;

    // SVD分解性能（最慢但最稳定）
    start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        JacobiSVD<MatrixXd> svd_perf(A_large, ComputeThinU | ComputeThinV);
        VectorXd x = svd_perf.solve(b_large);
    }
    end = high_resolution_clock::now();
    auto duration_svd = duration_cast<microseconds>(end - start);
    cout << "SVD分解平均耗时: " << duration_svd.count() / iterations << " 微秒" << endl;

    cout << "\n性能排序（从快到慢）:" << endl;
    cout << "1. Cholesky（对称正定）" << endl;
    cout << "2. LU（一般方阵）" << endl;
    cout << "3. QR（超定系统）" << endl;
    cout << "4. SVD（最稳定但最慢）" << endl;

    // ========== 5. 分解方法选择指南 ==========
    cout << "\n--- 5. 分解方法选择指南 ---" << endl;

    cout << "选择建议:" << endl;
    cout << "• 对称正定矩阵 → Cholesky（最快）" << endl;
    cout << "• 一般方阵 → LU（平衡速度和稳定性）" << endl;
    cout << "• 超定系统（m>n） → QR（最小二乘）" << endl;
    cout << "• 病态矩阵 → SVD（最稳定）" << endl;
    cout << "• 需要特征值 → 特征值分解" << endl;
    cout << "• 需要条件数 → SVD" << endl;

    cout << "\n=== 矩阵分解对比示例完成 ===" << endl;
    cin.get();
    return 0;
}
