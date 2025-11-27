#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen基本使用示例 ===" << endl;

    // 创建向量
    VectorXd v(3);
    v << 1.0, 2.0, 3.0;
    cout << "向量v:\n" << v << endl;

    // 创建矩阵
    MatrixXd A(3, 3);
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
    cout << "矩阵A:\n" << A << endl;

    // 矩阵转置
    MatrixXd A_T = A.transpose();
    cout << "矩阵A的转置:\n" << A_T << endl;

    // 矩阵乘法
    MatrixXd B = A * A_T;
    cout << "矩阵A * A^T:\n" << B << endl;

    // 向量点积
    double dot_product = v.dot(v);
    cout << "向量v的点积: " << dot_product << endl;

    // 矩阵行列式
    double det = A.determinant();
    cout << "矩阵A的行列式: " << det << endl;

    // 矩阵逆
    MatrixXd A_inv = A.inverse();
    cout << "矩阵A的逆:\n" << A_inv << endl;

    // 验证逆矩阵
    MatrixXd identity = A * A_inv;
    cout << "A * A^(-1) (应接近单位矩阵):\n" << identity << endl;

    cin.get();
    return 0;
}
