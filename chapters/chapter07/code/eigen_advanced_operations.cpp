#include <iostream>
#include <Eigen/Dense>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen高级矩阵运算示例 ===" << endl;

    Matrix3d A, B;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;
    B << 2, 0, 1,
         1, 1, 0,
         0, 1, 2;

    // ========== 矩阵幂运算 ==========
    cout << "\n1. 矩阵幂运算:" << endl;
    Matrix3d A_squared = A * A;
    cout << "A^2:\n" << A_squared << endl;

    Matrix3d A_cubed = A * A * A;
    cout << "A^3:\n" << A_cubed << endl;

    // ========== 矩阵求逆 ==========
    cout << "\n2. 矩阵求逆:" << endl;
    Matrix3d C;
    C << 2, -1, 0,
         -1, 2, -1,
         0, -1, 2;

    Matrix3d C_inv = C.inverse();
    cout << "矩阵C:\n" << C << endl;
    cout << "C的逆矩阵:\n" << C_inv << endl;
    cout << "C * C^(-1):\n" << C * C_inv << endl;

    // ========== 矩阵的幂和指数 ==========
    cout << "\n3. 矩阵的幂和指数:" << endl;
    Matrix3d D = Matrix3d::Identity();
    D << 1, 0.5, 0,
         0, 1, 0.5,
         0, 0, 1;

    // 计算 (I + A)^n 的近似
    Matrix3d I = Matrix3d::Identity();
    Matrix3d approx = I + D + 0.5 * D * D;
    cout << "近似 exp(D):\n" << approx << endl;

    // ========== 矩阵的Hadamard积 ==========
    cout << "\n4. Hadamard积 (元素级乘法):" << endl;
    Matrix3d H = A.cwiseProduct(B);
    cout << "A .* B:\n" << H << endl;

    // ========== 矩阵的Kronecker积 ==========
    cout << "\n5. Kronecker积:" << endl;
    Matrix2d E, F;
    E << 1, 2,
         3, 4;
    F << 5, 6,
         7, 8;

    // Kronecker积需要手动实现或使用特殊函数
    Matrix4d kron = Matrix4d::Zero();
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            kron.block(i*2, j*2, 2, 2) = E(i, j) * F;
        }
    }
    cout << "E ⊗ F:\n" << kron << endl;

    // ========== 矩阵的Schur积 ==========
    cout << "\n6. Schur积 (逐元素运算):" << endl;
    Matrix3d S1 = A.cwiseAbs2();  // 每个元素的平方
    cout << "A的每个元素平方:\n" << S1 << endl;

    Matrix3d S2 = A.cwiseSqrt();  // 每个元素的平方根
    cout << "A的每个元素平方根:\n" << S2 << endl;

    Matrix3d S3 = A.array().exp().matrix();  // 每个元素的指数
    cout << "exp(A) (逐元素):\n" << S3 << endl;

    Matrix3d S4 = A.array().log().matrix();  // 每个元素的对数
    cout << "log(A) (逐元素):\n" << S4 << endl;

    // ========== 矩阵的逐元素比较 ==========
    cout << "\n7. 逐元素比较:" << endl;
    Matrix3d comp = (A.array() > 5).cast<double>();
    cout << "A > 5 (逐元素):\n" << comp << endl;

    Matrix3d comp2 = (A.array() == B.array()).cast<double>();
    cout << "A == B (逐元素):\n" << comp2 << endl;

    // ========== 矩阵的逐元素函数 ==========
    cout << "\n8. 逐元素函数:" << endl;
    Matrix3d func1 = A.array().sin();  // 正弦
    cout << "sin(A):\n" << func1 << endl;

    Matrix3d func2 = A.array().cos();  // 余弦
    cout << "cos(A):\n" << func2 << endl;

    Matrix3d func3 = A.array().tan();  // 正切
    cout << "tan(A):\n" << func3 << endl;

    // ========== 矩阵的逐元素条件运算 ==========
    cout << "\n9. 逐元素条件运算:" << endl;
    Matrix3d cond = (A.array() > 5).select(Matrix3d::Ones(), Matrix3d::Zero());
    cout << "A > 5 ? 1 : 0:\n" << cond << endl;

    // ========== 矩阵的逐元素归约 ==========
    cout << "\n10. 逐元素归约:" << endl;
    double sum_all = A.sum();
    double prod_all = A.prod();
    double mean_all = A.mean();

    cout << "所有元素和: " << sum_all << endl;
    cout << "所有元素积: " << prod_all << endl;
    cout << "所有元素均值: " << mean_all << endl;

    // ========== 矩阵的逐行/逐列归约 ==========
    cout << "\n11. 逐行/逐列归约:" << endl;
    Vector3d row_max = A.rowwise().maxCoeff();
    Vector3d col_min = A.colwise().minCoeff();

    cout << "每行最大值: " << row_max.transpose() << endl;
    cout << "每列最小值: " << col_min.transpose() << endl;

    cin.get();
    return 0;
}
