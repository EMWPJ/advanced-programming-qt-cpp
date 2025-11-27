#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen线性方程求解示例 ===" << endl;

    // 创建系数矩阵A和右端向量b
    MatrixXd A(3, 3);
    VectorXd b(3);

    A << 2, -1, 1,
         -3, 2, 1,
         1, 1, 4;

    b << 8, -11, 11;

    cout << "系数矩阵A:\n" << A << endl;
    cout << "右端向量b:\n" << b << endl;

    // 直接求逆法求解
    VectorXd x1 = A.inverse() * b;
    cout << "直接求逆法解x:\n" << x1 << endl;

    // LU分解求解
    VectorXd x2 = A.lu().solve(b);
    cout << "LU分解法解x:\n" << x2 << endl;

    // QR分解求解
    VectorXd x3 = A.householderQr().solve(b);
    cout << "QR分解法解x:\n" << x3 << endl;

    // Cholesky分解（适用于对称正定矩阵）
    MatrixXd sym_A(3, 3);
    sym_A << 4, 2, 1,
             2, 5, 3,
             1, 3, 6;

    VectorXd sym_b(3);
    sym_b << 7, 11, 13;

    cout << "\n对称正定矩阵:\n" << sym_A << endl;
    cout << "右端向量:\n" << sym_b << endl;

    VectorXd x4 = sym_A.llt().solve(sym_b);
    cout << "Cholesky分解法解x:\n" << x4 << endl;

    // 验证解的正确性
    VectorXd residual1 = A * x1 - b;
    cout << "\n直接求逆法残差:\n" << residual1 << endl;

    VectorXd residual2 = A * x2 - b;
    cout << "LU分解法残差:\n" << residual2 << endl;

    VectorXd residual3 = A * x3 - b;
    cout << "QR分解法残差:\n" << residual3 << endl;

    VectorXd residual4 = sym_A * x4 - sym_b;
    cout << "Cholesky分解法残差:\n" << residual4 << endl;

    cin.get();
    return 0;
}
