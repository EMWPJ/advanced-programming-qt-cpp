#include <iostream>
#include <chrono>
#include <Eigen/Dense>

// 定义宏以使用MKL作为Eigen的后端
#define EIGEN_USE_MKL_ALL
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen使用MKL加速示例 ===" << endl;

    // 设置矩阵大小
    const int size = 1000;

    // 创建随机矩阵
    MatrixXd A = MatrixXd::Random(size, size);
    MatrixXd B = MatrixXd::Random(size, size);
    MatrixXd C(size, size);

    cout << "矩阵大小: " << size << "x" << size << endl;

    // 测试1: 矩阵乘法（使用MKL加速）
    auto start = high_resolution_clock::now();

    C = A * B;  // Eigen会自动使用MKL的BLAS进行加速

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "矩阵乘法耗时（MKL加速）: " << duration.count() << " 毫秒" << endl;

    // 测试2: 矩阵求逆（使用MKL的LAPACK）
    MatrixXd A_copy = A;

    start = high_resolution_clock::now();

    MatrixXd A_inv = A_copy.inverse();  // 使用MKL的LAPACK加速

    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "矩阵求逆耗时（MKL加速）: " << duration.count() << " 毫秒" << endl;

    // 验证结果
    MatrixXd identity = A * A_inv;
    double max_error = (identity - MatrixXd::Identity(size, size)).cwiseAbs().maxCoeff();
    cout << "求逆验证最大误差: " << max_error << endl;

    // 测试3: 特征值分解（使用MKL）
    MatrixXd sym_A = A.transpose() * A;  // 对称矩阵

    start = high_resolution_clock::now();

    SelfAdjointEigenSolver<MatrixXd> eigen_solver(sym_A);
    VectorXd eigenvalues = eigen_solver.eigenvalues();

    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "特征值分解耗时（MKL加速）: " << duration.count() << " 毫秒" << endl;
    cout << "最大特征值: " << eigenvalues.maxCoeff() << endl;
    cout << "最小特征值: " << eigenvalues.minCoeff() << endl;

    // 测试4: 线性方程组求解（使用MKL的LAPACK）
    VectorXd b = VectorXd::Random(size);
    VectorXd x(size);

    start = high_resolution_clock::now();

    x = A_copy.lu().solve(b);  // 使用MKL的LAPACK加速

    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "线性方程组求解耗时（MKL加速）: " << duration.count() << " 毫秒" << endl;

    // 验证解的正确性
    VectorXd residual = A_copy * x - b;
    double residual_norm = residual.norm();
    cout << "残差范数: " << residual_norm << endl;

    // 显示MKL版本信息
    cout << "\nMKL版本信息:" << endl;
    MKLVersion mkl_version;
    mkl_get_version(&mkl_version);
    cout << "版本: " << mkl_version.MajorVersion << "."
         << mkl_version.MinorVersion << "."
         << mkl_version.UpdateVersion << endl;
    cout << "产品状态: " << mkl_version.ProductStatus << endl;

    cin.get();
    return 0;
}
