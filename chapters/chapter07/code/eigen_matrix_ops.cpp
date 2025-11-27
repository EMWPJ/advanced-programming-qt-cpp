#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵运算示例 ===" << endl;

    // 创建两个矩阵
    MatrixXd A(3, 3);
    MatrixXd B(3, 3);

    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 10;

    B << 2, 0, 1,
         1, 1, 0,
         0, 1, 2;

    cout << "矩阵A:\n" << A << endl;
    cout << "矩阵B:\n" << B << endl;

    // 矩阵加法
    MatrixXd C = A + B;
    cout << "A + B:\n" << C << endl;

    // 矩阵减法
    MatrixXd D = A - B;
    cout << "A - B:\n" << D << endl;

    // 标量乘法
    MatrixXd E = 2.0 * A;
    cout << "2 * A:\n" << E << endl;

    // 矩阵乘法
    MatrixXd F = A * B;
    cout << "A * B:\n" << F << endl;

    // 元素级乘法
    MatrixXd G = A.cwiseProduct(B);
    cout << "A .* B (元素级乘法):\n" << G << endl;

    // 矩阵的Frobenius范数
    double norm_A = A.norm();
    cout << "矩阵A的Frobenius范数: " << norm_A << endl;

    // 矩阵的迹
    double trace_A = A.trace();
    cout << "矩阵A的迹: " << trace_A << endl;

    // 矩阵的特征值
    EigenSolver<MatrixXd> eigen_solver(A);
    VectorXd eigenvalues = eigen_solver.eigenvalues().real();
    cout << "矩阵A的特征值:\n" << eigenvalues << endl;

    cin.get();
    return 0;
}
