#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>
#include <cmath>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen线性方程组求解完整示例 ===" << endl;

    // ========== 1. 基本线性方程组求解 ==========
    cout << "\n--- 1. 基本线性方程组 Ax = b ---" << endl;

    MatrixXd A(3, 3);
    VectorXd b(3);

    A << 2, -1, 1,
         -3, 2, 1,
         1, 1, 4;

    b << 8, -11, 11;

    cout << "系数矩阵A:\n" << A << endl;
    cout << "右端向量b:\n" << b << endl;

    // 方法1: 直接求逆（不推荐，数值不稳定）
    VectorXd x1 = A.inverse() * b;
    cout << "\n直接求逆法解:\n" << x1 << endl;

    // 方法2: LU分解（推荐用于一般方阵）
    PartialPivLU<MatrixXd> lu(A);
    VectorXd x2 = lu.solve(b);
    cout << "LU分解法解:\n" << x2 << endl;

    // 方法3: QR分解（更稳定）
    HouseholderQR<MatrixXd> qr(A);
    VectorXd x3 = qr.solve(b);
    cout << "QR分解法解:\n" << x3 << endl;

    // 验证解
    VectorXd residual = A * x2 - b;
    cout << "残差范数: " << residual.norm() << endl;

    // ========== 2. 超定系统（最小二乘） ==========
    cout << "\n--- 2. 超定系统（最小二乘）---" << endl;

    // 超定系统：方程数 > 未知数个数
    MatrixXd A_over(5, 3);  // 5个方程，3个未知数
    VectorXd b_over(5);

    A_over << 1, 1, 1,
              1, 2, 4,
              1, 3, 9,
              1, 4, 16,
              1, 5, 25;

    b_over << 2.1, 3.9, 6.1, 8.0, 10.2;

    cout << "超定系统 A (5x3):\n" << A_over << endl;
    cout << "右端向量 b:\n" << b_over << endl;

    // QR分解求解最小二乘问题
    HouseholderQR<MatrixXd> qr_over(A_over);
    VectorXd x_over = qr_over.solve(b_over);
    cout << "最小二乘解:\n" << x_over << endl;

    // 计算残差
    VectorXd residual_over = A_over * x_over - b_over;
    cout << "残差向量:\n" << residual_over << endl;
    cout << "残差平方和: " << residual_over.squaredNorm() << endl;

    // ========== 3. 欠定系统 ==========
    cout << "\n--- 3. 欠定系统（方程数 < 未知数）---" << endl;

    MatrixXd A_under(2, 4);  // 2个方程，4个未知数
    VectorXd b_under(2);

    A_under << 1, 2, 3, 4,
               2, 3, 4, 5;

    b_under << 10, 14;

    cout << "欠定系统 A (2x4):\n" << A_under << endl;
    cout << "右端向量 b:\n" << b_under << endl;

    // 使用SVD求解欠定系统（最小范数解）
    JacobiSVD<MatrixXd> svd_under(A_under, ComputeThinU | ComputeThinV);
    VectorXd x_under = svd_under.solve(b_under);
    cout << "最小范数解:\n" << x_under << endl;
    cout << "解的范数: " << x_under.norm() << endl;

    // ========== 4. 对称正定矩阵（Cholesky分解） ==========
    cout << "\n--- 4. 对称正定矩阵（Cholesky分解）---" << endl;

    // 构造对称正定矩阵
    MatrixXd A_sym(3, 3);
    A_sym << 4, 2, 1,
             2, 5, 3,
             1, 3, 6;

    VectorXd b_sym(3);
    b_sym << 7, 11, 13;

    cout << "对称正定矩阵A:\n" << A_sym << endl;

    // Cholesky分解（最快）
    LLT<MatrixXd> chol(A_sym);
    if (chol.info() == Success) {
        VectorXd x_chol = chol.solve(b_sym);
        cout << "Cholesky分解解:\n" << x_chol << endl;
    } else {
        cout << "Cholesky分解失败" << endl;
    }

    // ========== 5. 多右端项求解 ==========
    cout << "\n--- 5. 多右端项求解 AX = B ---" << endl;

    MatrixXd B(3, 2);  // 两个右端向量
    B << 8, 1,
         -11, 2,
         11, 3;

    cout << "右端矩阵B:\n" << B << endl;

    // 重用LU分解求解多个右端项
    PartialPivLU<MatrixXd> lu_multi(A);
    MatrixXd X = lu_multi.solve(B);
    cout << "解矩阵X:\n" << X << endl;

    // 验证
    MatrixXd residual_multi = A * X - B;
    cout << "残差矩阵的Frobenius范数: " << residual_multi.norm() << endl;

    // ========== 6. 稀疏矩阵求解 ==========
    cout << "\n--- 6. 稀疏矩阵线性方程组求解 ---" << endl;

    // 创建稀疏矩阵（三对角矩阵）
    int n = 10;
    SparseMatrix<double> A_sparse(n, n);
    vector<Triplet<double>> triplets;

    for (int i = 0; i < n; ++i) {
        if (i > 0) triplets.push_back(Triplet<double>(i, i-1, -1));
        triplets.push_back(Triplet<double>(i, i, 2));
        if (i < n-1) triplets.push_back(Triplet<double>(i, i+1, -1));
    }

    A_sparse.setFromTriplets(triplets.begin(), triplets.end());

    VectorXd b_sparse = VectorXd::Ones(n);

    cout << "稀疏矩阵大小: " << n << "x" << n << endl;
    cout << "非零元素数: " << A_sparse.nonZeros() << endl;

    // 稀疏LU分解
    SparseLU<SparseMatrix<double>> sparse_lu;
    sparse_lu.compute(A_sparse);

    if (sparse_lu.info() == Success) {
        VectorXd x_sparse = sparse_lu.solve(b_sparse);
        cout << "稀疏矩阵解的前5个元素:\n" << x_sparse.head(5) << endl;

        // 验证
        VectorXd residual_sparse = A_sparse * x_sparse - b_sparse;
        cout << "残差范数: " << residual_sparse.norm() << endl;
    }

    // ========== 7. 病态矩阵处理 ==========
    cout << "\n--- 7. 病态矩阵处理 ---" << endl;

    // 构造病态矩阵（Hilbert矩阵）
    int m = 5;
    MatrixXd A_ill(m, m);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            A_ill(i, j) = 1.0 / (i + j + 1.0);
        }
    }

    VectorXd b_ill = VectorXd::Ones(m);

    // 计算条件数
    JacobiSVD<MatrixXd> svd_ill(A_ill);
    VectorXd singular_values = svd_ill.singularValues();
    double cond = singular_values(0) / singular_values(m-1);
    cout << "矩阵条件数: " << cond << endl;

    if (cond > 1e12) {
        cout << "警告：矩阵病态，使用SVD求解" << endl;
        VectorXd x_ill = svd_ill.solve(b_ill);
        cout << "SVD解:\n" << x_ill << endl;
    } else {
        VectorXd x_ill = A_ill.lu().solve(b_ill);
        cout << "LU解:\n" << x_ill << endl;
    }

    cin.get();
    return 0;
}
