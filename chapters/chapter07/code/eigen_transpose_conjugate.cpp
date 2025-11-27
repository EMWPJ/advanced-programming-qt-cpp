#include <iostream>
#include <Eigen/Dense>
#include <complex>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵转置与共轭示例 ===" << endl;

    // ========== 矩阵转置 ==========
    cout << "\n1. 矩阵转置:" << endl;

    Matrix3d A;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    cout << "矩阵A:\n" << A << endl;

    // transpose() - 转置（返回表达式，不复制）
    Matrix3d A_T = A.transpose();
    cout << "A.transpose():\n" << A_T << endl;

    // 验证转置性质
    Matrix3d verify = A.transpose().transpose();
    cout << "(A^T)^T = A:\n" << verify << endl;

    // ========== 转置的原地操作 ==========
    cout << "\n2. 转置的原地操作:" << endl;

    MatrixXd B(3, 3);
    B << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    // transposeInPlace() - 原地转置
    B.transposeInPlace();
    cout << "B.transposeInPlace():\n" << B << endl;

    // ========== 共轭转置 ==========
    cout << "\n3. 共轭转置 (复数矩阵):" << endl;

    MatrixXcd C(3, 3);
    C << complex<double>(1, 1), complex<double>(2, 2), complex<double>(3, 3),
         complex<double>(4, 4), complex<double>(5, 5), complex<double>(6, 6),
         complex<double>(7, 7), complex<double>(8, 8), complex<double>(9, 9);

    cout << "复数矩阵C:\n" << C << endl;

    // adjoint() - 共轭转置
    MatrixXcd C_H = C.adjoint();
    cout << "C.adjoint() (共轭转置):\n" << C_H << endl;

    // conjugate() - 共轭（不转置）
    MatrixXcd C_conj = C.conjugate();
    cout << "C.conjugate() (共轭):\n" << C_conj << endl;

    // ========== 转置在表达式中的使用 ==========
    cout << "\n4. 转置在表达式中的使用:" << endl;

    Matrix3d D, E;
    D << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
    E << 2, 0, 1,
         1, 1, 0,
         0, 1, 2;

    // A^T * B
    Matrix3d result1 = D.transpose() * E;
    cout << "D^T * E:\n" << result1 << endl;

    // A * B^T
    Matrix3d result2 = D * E.transpose();
    cout << "D * E^T:\n" << result2 << endl;

    // ========== 向量转置 ==========
    cout << "\n5. 向量转置:" << endl;

    Vector3d v;
    v << 1, 2, 3;

    cout << "列向量v:\n" << v << endl;
    cout << "行向量v^T: " << v.transpose() << endl;

    // 行向量转置为列向量
    RowVector3d row_v;
    row_v << 1, 2, 3;
    Vector3d col_v = row_v.transpose();
    cout << "行向量转置为列向量:\n" << col_v << endl;

    // ========== 转置的性能考虑 ==========
    cout << "\n6. 转置的性能考虑:" << endl;

    MatrixXd F(1000, 1000);
    F.setRandom();

    // transpose()返回表达式，延迟求值
    // 如果结果赋值给相同类型，会进行优化
    MatrixXd F_T = F.transpose();  // 会实际计算转置

    // 在表达式中使用转置，Eigen会自动优化
    VectorXd x(1000);
    x.setRandom();
    VectorXd y = F.transpose() * x;  // 优化：避免实际转置

    cout << "转置操作已优化" << endl;

    // ========== 对称矩阵 ==========
    cout << "\n7. 对称矩阵:" << endl;

    Matrix3d symmetric;
    symmetric << 1, 2, 3,
                 2, 4, 5,
                 3, 5, 6;

    cout << "对称矩阵:\n" << symmetric << endl;
    cout << "转置后:\n" << symmetric.transpose() << endl;
    cout << "是否对称: " << (symmetric.isApprox(symmetric.transpose()) ? "是" : "否") << endl;

    cin.get();
    return 0;
}
