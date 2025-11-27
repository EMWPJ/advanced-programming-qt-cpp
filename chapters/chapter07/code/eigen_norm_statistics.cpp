#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵范数与统计示例 ===" << endl;

    Matrix3d A;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    cout << "矩阵A:\n" << A << endl;

    // ========== 矩阵范数 ==========
    cout << "\n1. 矩阵范数:" << endl;

    // Frobenius范数（默认）
    double frobenius_norm = A.norm();
    cout << "Frobenius范数 ||A||_F = " << frobenius_norm << endl;

    // L2范数（最大奇异值）
    double l2_norm = A.norm();
    cout << "L2范数 ||A||_2 = " << l2_norm << endl;

    // L1范数（列和的最大值）
    double l1_norm = A.colwise().lpNorm<1>().maxCoeff();
    cout << "L1范数 ||A||_1 = " << l1_norm << endl;

    // 无穷范数（行和的最大值）
    double inf_norm = A.rowwise().lpNorm<1>().maxCoeff();
    cout << "无穷范数 ||A||_∞ = " << inf_norm << endl;

    // ========== 向量范数 ==========
    cout << "\n2. 向量范数:" << endl;

    Vector3d v;
    v << 3, 4, 0;

    cout << "向量v: " << v.transpose() << endl;
    cout << "L2范数 ||v||_2 = " << v.norm() << endl;
    cout << "L1范数 ||v||_1 = " << v.lpNorm<1>() << endl;
    cout << "L∞范数 ||v||_∞ = " << v.lpNorm<Infinity>() << endl;
    cout << "平方范数 ||v||^2 = " << v.squaredNorm() << endl;

    // ========== 矩阵统计 ==========
    cout << "\n3. 矩阵统计:" << endl;

    // 迹（trace）
    double trace = A.trace();
    cout << "迹 tr(A) = " << trace << endl;

    // 行列式
    double determinant = A.determinant();
    cout << "行列式 det(A) = " << determinant << endl;

    // 矩阵的秩（需要SVD）
    JacobiSVD<Matrix3d> svd(A);
    int rank = svd.rank();
    cout << "矩阵的秩 = " << rank << endl;

    // ========== 元素统计 ==========
    cout << "\n4. 元素统计:" << endl;

    // 最大值和最小值
    double max_val = A.maxCoeff();
    double min_val = A.minCoeff();
    cout << "最大值: " << max_val << endl;
    cout << "最小值: " << min_val << endl;

    // 最大值和最小值的位置
    MatrixXd::Index maxRow, maxCol, minRow, minCol;
    A.maxCoeff(&maxRow, &maxCol);
    A.minCoeff(&minRow, &minCol);
    cout << "最大值位置: (" << maxRow << ", " << maxCol << ")" << endl;
    cout << "最小值位置: (" << minRow << ", " << minCol << ")" << endl;

    // 行和列的和
    VectorXd row_sums = A.rowwise().sum();
    VectorXd col_sums = A.colwise().sum();
    cout << "行和: " << row_sums.transpose() << endl;
    cout << "列和: " << col_sums.transpose() << endl;

    // 行和列的平均值
    VectorXd row_means = A.rowwise().mean();
    VectorXd col_means = A.colwise().mean();
    cout << "行平均值: " << row_means.transpose() << endl;
    cout << "列平均值: " << col_means.transpose() << endl;

    // ========== 矩阵比较 ==========
    cout << "\n5. 矩阵比较:" << endl;

    Matrix3d B;
    B << 1.0001, 2.0001, 3.0001,
         4.0001, 5.0001, 6.0001,
         7.0001, 8.0001, 9.0001;

    cout << "矩阵B:\n" << B << endl;

    // 近似相等（考虑浮点误差）
    double tolerance = 1e-3;
    bool is_approx = A.isApprox(B, tolerance);
    cout << "A ≈ B (tolerance=" << tolerance << "): "
         << (is_approx ? "是" : "否") << endl;

    // 是否远小于
    Matrix3d C = A * 0.001;
    bool is_much_smaller = C.isMuchSmallerThan(A);
    cout << "C << A: " << (is_much_smaller ? "是" : "否") << endl;

    // ========== 条件数 ==========
    cout << "\n6. 条件数:" << endl;

    // 条件数 = 最大奇异值 / 最小奇异值
    JacobiSVD<Matrix3d> svd_A(A);
    Vector3d singular_values = svd_A.singularValues();
    double condition_number = singular_values(0) / singular_values(singular_values.size()-1);
    cout << "奇异值: " << singular_values.transpose() << endl;
    cout << "条件数: " << condition_number << endl;

    // ========== 矩阵的乘积 ==========
    cout << "\n7. 矩阵乘积统计:" << endl;

    Vector3d u, w;
    u << 1, 2, 3;
    w << 4, 5, 6;

    // 点积
    double dot_product = u.dot(w);
    cout << "u · w = " << dot_product << endl;

    // 外积
    Matrix3d outer_product = u * w.transpose();
    cout << "u * w^T:\n" << outer_product << endl;

    // 叉积（3维向量）
    Vector3d cross_product = u.cross(w);
    cout << "u × w: " << cross_product.transpose() << endl;

    cin.get();
    return 0;
}
