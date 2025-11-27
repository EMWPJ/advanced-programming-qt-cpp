#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen特征值与特征向量示例 ===" << endl;

    // 创建一个对称矩阵
    MatrixXd A(3, 3);
    A << 4, 2, 1,
         2, 5, 3,
         1, 3, 6;

    cout << "矩阵A:\n" << A << endl;

    // 使用EigenSolver计算特征值和特征向量
    EigenSolver<MatrixXd> eigen_solver(A);

    // 获取特征值（复数形式）
    VectorXcd eigenvalues = eigen_solver.eigenvalues();
    cout << "特征值（复数）:\n" << eigenvalues << endl;

    // 获取特征值（实数部分）
    VectorXd eigenvalues_real = eigenvalues.real();
    cout << "特征值（实数部分）:\n" << eigenvalues_real << endl;

    // 获取特征向量
    MatrixXcd eigenvectors = eigen_solver.eigenvectors();
    cout << "特征向量:\n" << eigenvectors << endl;

    // 验证特征方程 A*v = λ*v
    cout << "\n验证特征方程 A*v = λ*v:" << endl;
    for (int i = 0; i < 3; ++i) {
        VectorXcd v = eigenvectors.col(i);
        complex<double> lambda = eigenvalues(i);

        VectorXcd Av = A.cast<complex<double>>() * v;
        VectorXcd lambda_v = lambda * v;

        cout << "第" << i+1 << "个特征值: " << lambda << endl;
        cout << "A*v - λ*v:\n" << (Av - lambda_v) << endl;
        cout << endl;
    }

    // 使用SelfAdjointEigenSolver处理实对称矩阵
    SelfAdjointEigenSolver<MatrixXd> sa_eigen_solver(A);

    if (sa_eigen_solver.info() == Success) {
        VectorXd sa_eigenvalues = sa_eigen_solver.eigenvalues();
        MatrixXd sa_eigenvectors = sa_eigen_solver.eigenvectors();

        cout << "使用SelfAdjointEigenSolver:" << endl;
        cout << "特征值:\n" << sa_eigenvalues << endl;
        cout << "特征向量:\n" << sa_eigenvectors << endl;

        // 验证正交性
        MatrixXd VTV = sa_eigenvectors.transpose() * sa_eigenvectors;
        cout << "特征向量正交性验证 V^T*V:\n" << VTV << endl;
    }

    cin.get();
    return 0;
}
