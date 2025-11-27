#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵算术运算示例 ===" << endl;

    // 创建两个矩阵
    Matrix3d A, B;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
    B << 2, 0, 1,
         1, 1, 0,
         0, 1, 2;

    cout << "矩阵A:\n" << A << endl;
    cout << "矩阵B:\n" << B << endl;

    // ========== 矩阵加法 ==========
    cout << "\n1. 矩阵加法:" << endl;
    Matrix3d C = A + B;
    cout << "A + B:\n" << C << endl;

    // ========== 矩阵减法 ==========
    cout << "\n2. 矩阵减法:" << endl;
    Matrix3d D = A - B;
    cout << "A - B:\n" << D << endl;

    // ========== 标量乘法 ==========
    cout << "\n3. 标量乘法:" << endl;
    Matrix3d E = 2.0 * A;
    cout << "2 * A:\n" << E << endl;
    Matrix3d F = A * 3.0;
    cout << "A * 3:\n" << F << endl;

    // ========== 矩阵乘法 ==========
    cout << "\n4. 矩阵乘法:" << endl;
    Matrix3d G = A * B;
    cout << "A * B:\n" << G << endl;

    // ========== 矩阵-向量乘法 ==========
    cout << "\n5. 矩阵-向量乘法:" << endl;
    Vector3d v;
    v << 1, 2, 3;
    Vector3d result = A * v;
    cout << "A * v:\n" << result << endl;

    // ========== 元素级运算 ==========
    cout << "\n6. 元素级运算:" << endl;
    Matrix3d H = A.cwiseProduct(B);  // 元素级乘法
    cout << "A .* B (元素级乘法):\n" << H << endl;

    Matrix3d I = A.cwiseQuotient(B);  // 元素级除法
    cout << "A ./ B (元素级除法):\n" << I << endl;

    Matrix3d J = A.cwiseAbs();  // 绝对值
    cout << "abs(A):\n" << J << endl;

    Matrix3d K = A.cwiseSqrt();  // 平方根
    cout << "sqrt(A):\n" << K << endl;

    // ========== 复合运算 ==========
    cout << "\n7. 复合运算:" << endl;
    Matrix3d L = 2 * A + 3 * B;
    cout << "2*A + 3*B:\n" << L << endl;

    Matrix3d M = A.transpose() * B;
    cout << "A^T * B:\n" << M << endl;

    // ========== 就地运算 ==========
    cout << "\n8. 就地运算 (in-place):" << endl;
    Matrix3d N = A;
    N += B;  // 等价于 N = N + B
    cout << "N += B:\n" << N << endl;

    Matrix3d O = A;
    O *= 2.0;  // 等价于 O = O * 2
    cout << "O *= 2:\n" << O << endl;

    cin.get();
    return 0;
}
