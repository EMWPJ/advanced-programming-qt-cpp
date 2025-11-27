#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// Eigen综合用法示例：展示各种实用技巧

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen综合用法示例 ===" << endl;

    // ========== 1. 矩阵和向量的基本操作 ==========
    cout << "\n--- 1. 矩阵和向量的基本操作 ---" << endl;

    Matrix3d A;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    Vector3d v;
    v << 1, 2, 3;

    cout << "矩阵A:\n" << A << endl;
    cout << "向量v:\n" << v << endl;
    cout << "A * v:\n" << A * v << endl;
    cout << "v.transpose() * A:\n" << v.transpose() * A << endl;
    cout << "v.dot(v): " << v.dot(v) << endl;
    cout << "v.norm(): " << v.norm() << endl;
    cout << "v.normalized():\n" << v.normalized() << endl;

    // ========== 2. 矩阵的常用操作 ==========
    cout << "\n--- 2. 矩阵的常用操作 ---" << endl;

    Matrix3d B = Matrix3d::Random();
    cout << "随机矩阵B:\n" << B << endl;

    // 转置
    cout << "B转置:\n" << B.transpose() << endl;

    // 逆矩阵
    cout << "B的逆:\n" << B.inverse() << endl;

    // 行列式
    cout << "det(B): " << B.determinant() << endl;

    // 迹
    cout << "trace(B): " << B.trace() << endl;

    // 范数
    cout << "Frobenius范数: " << B.norm() << endl;
    cout << "L1范数: " << B.colwise().sum().maxCoeff() << endl;
    cout << "L∞范数: " << B.rowwise().sum().maxCoeff() << endl;

    // ========== 3. 矩阵块操作的高级用法 ==========
    cout << "\n--- 3. 矩阵块操作的高级用法 ---" << endl;

    MatrixXd M = MatrixXd::Random(5, 5);
    cout << "矩阵M (5x5):\n" << M << endl;

    // 提取子矩阵
    MatrixXd top_left = M.topLeftCorner(2, 2);
    cout << "左上角2x2:\n" << top_left << endl;

    // 提取行和列
    VectorXd first_row = M.row(0);
    VectorXd first_col = M.col(0);
    cout << "第一行:\n" << first_row << endl;
    cout << "第一列:\n" << first_col << endl;

    // 提取中间块
    MatrixXd center = M.block(1, 1, 3, 3);
    cout << "中心3x3块:\n" << center << endl;

    // 修改块
    M.block(0, 0, 2, 2) = Matrix2d::Identity();
    cout << "修改左上角为单位矩阵后:\n" << M << endl;

    // ========== 4. 元素级操作 ==========
    cout << "\n--- 4. 元素级操作 ---" << endl;

    Matrix3d C = Matrix3d::Random();
    Matrix3d D = Matrix3d::Random();

    cout << "矩阵C:\n" << C << endl;
    cout << "矩阵D:\n" << D << endl;

    // 元素级运算
    cout << "C + D (元素级):\n" << C + D << endl;
    cout << "C * D (元素级):\n" << C.cwiseProduct(D) << endl;
    cout << "C / D (元素级):\n" << C.cwiseQuotient(D) << endl;

    // 元素级函数
    cout << "exp(C) (元素级):\n" << C.array().exp().matrix() << endl;
    cout << "log(|C|) (元素级):\n" << C.array().abs().log().matrix() << endl;
    cout << "sqrt(|C|) (元素级):\n" << C.array().abs().sqrt().matrix() << endl;

    // 元素级比较
    Matrix3d mask = (C.array() > 0).cast<double>();
    cout << "C > 0 的掩码:\n" << mask << endl;

    // ========== 5. 矩阵分解和求解 ==========
    cout << "\n--- 5. 矩阵分解和求解 ---" << endl;

    Matrix3d E = Matrix3d::Random();
    E = E.transpose() * E;  // 对称正定矩阵
    Vector3d b = Vector3d::Random();

    cout << "系数矩阵E:\n" << E << endl;
    cout << "右端向量b:\n" << b << endl;

    // LU分解
    Vector3d x_lu = E.lu().solve(b);
    cout << "LU分解解:\n" << x_lu << endl;

    // QR分解
    Vector3d x_qr = E.householderQr().solve(b);
    cout << "QR分解解:\n" << x_qr << endl;

    // Cholesky分解（对称正定）
    Vector3d x_chol = E.llt().solve(b);
    cout << "Cholesky分解解:\n" << x_chol << endl;

    // 验证
    Vector3d residual = E * x_chol - b;
    cout << "残差:\n" << residual << endl;

    // ========== 6. 特征值和特征向量 ==========
    cout << "\n--- 6. 特征值和特征向量 ---" << endl;

    Matrix3d F = Matrix3d::Random();
    F = F.transpose() * F;  // 对称矩阵

    SelfAdjointEigenSolver<Matrix3d> eigen_solver(F);
    Vector3d eigenvals = eigen_solver.eigenvalues();
    Matrix3d eigenvecs = eigen_solver.eigenvectors();

    cout << "矩阵F:\n" << F << endl;
    cout << "特征值:\n" << eigenvals << endl;
    cout << "特征向量:\n" << eigenvecs << endl;

    // 验证：F * v = λ * v
    Vector3d v1 = eigenvecs.col(0);
    double lambda1 = eigenvals(0);
    Vector3d Fv = F * v1;
    Vector3d lambda_v = lambda1 * v1;
    cout << "验证 F*v1 ≈ λ1*v1:" << endl;
    cout << "F*v1:\n" << Fv << endl;
    cout << "λ1*v1:\n" << lambda_v << endl;

    // ========== 7. 矩阵的统计操作 ==========
    cout << "\n--- 7. 矩阵的统计操作 ---" << endl;

    MatrixXd G = MatrixXd::Random(4, 5);
    cout << "矩阵G:\n" << G << endl;

    // 每列统计
    cout << "每列均值:\n" << G.colwise().mean() << endl;
    cout << "每列和:\n" << G.colwise().sum() << endl;
    cout << "每列最大值:\n" << G.colwise().maxCoeff() << endl;
    cout << "每列最小值:\n" << G.colwise().minCoeff() << endl;

    // 每行统计
    cout << "每行均值:\n" << G.rowwise().mean() << endl;
    cout << "每行和:\n" << G.rowwise().sum() << endl;

    // 全局统计
    cout << "全局最大值: " << G.maxCoeff() << endl;
    cout << "全局最小值: " << G.minCoeff() << endl;
    cout << "全局均值: " << G.mean() << endl;
    cout << "全局和: " << G.sum() << endl;

    // ========== 8. 矩阵重塑和重组 ==========
    cout << "\n--- 8. 矩阵重塑和重组 ---" << endl;

    VectorXd vec = VectorXd::LinSpaced(12, 1, 12);
    cout << "向量vec:\n" << vec << endl;

    // 重塑为矩阵
    MatrixXd H = Map<MatrixXd>(vec.data(), 3, 4);
    cout << "重塑为3x4矩阵:\n" << H << endl;

    // 转置重塑
    MatrixXd H_T = Map<MatrixXd>(vec.data(), 4, 3);
    cout << "重塑为4x3矩阵:\n" << H_T << endl;

    // 提取对角
    VectorXd diag = H.diagonal();
    cout << "对角元素:\n" << diag << endl;

    // ========== 9. 矩阵的特殊构造 ==========
    cout << "\n--- 9. 矩阵的特殊构造 ---" << endl;

    // 对角矩阵
    Vector3d diag_vals;
    diag_vals << 1, 2, 3;
    Matrix3d diag_mat = diag_vals.asDiagonal();
    cout << "对角矩阵:\n" << diag_mat << endl;

    // 外积
    Vector3d u, w;
    u << 1, 2, 3;
    w << 4, 5, 6;
    Matrix3d outer = u * w.transpose();
    cout << "外积 u * w^T:\n" << outer << endl;

    // Kronecker积（使用块操作模拟）
    Matrix2d I = Matrix2d::Identity();
    Matrix2d J;
    J << 1, 2, 3, 4;
    Matrix4d K;
    K << J(0,0)*I, J(0,1)*I,
         J(1,0)*I, J(1,1)*I;
    cout << "Kronecker积示例:\n" << K << endl;

    // ========== 10. 性能优化技巧 ==========
    cout << "\n--- 10. 性能优化技巧 ---" << endl;

    MatrixXd X = MatrixXd::Random(100, 100);
    MatrixXd Y = MatrixXd::Random(100, 100);
    MatrixXd Z(100, 100);

    // 使用noalias避免临时对象
    Z.noalias() = X * Y;
    cout << "使用noalias完成矩阵乘法" << endl;

    // 使用Map操作现有数据
    vector<double> data(100);
    for (int i = 0; i < 100; ++i) data[i] = i;
    Map<VectorXd> mapped_vec(data.data(), 100);
    cout << "使用Map操作vector数据，无需拷贝" << endl;

    // 固定大小矩阵（小矩阵）
    Matrix3d small_A = Matrix3d::Random();
    Matrix3d small_B = Matrix3d::Random();
    Matrix3d small_C = small_A * small_B;  // 栈分配，更快
    cout << "固定大小矩阵运算（栈分配）" << endl;

    // ========== 11. 几何变换（使用Geometry模块）==========
    cout << "\n--- 11. 几何变换 ---" << endl;

    // 旋转矩阵
    AngleAxisd rotation(M_PI / 4, Vector3d::UnitZ());
    Matrix3d R = rotation.toRotationMatrix();
    cout << "绕Z轴旋转45度的旋转矩阵:\n" << R << endl;

    // 旋转向量
    Vector3d point;
    point << 1, 0, 0;
    Vector3d rotated = R * point;
    cout << "点(1,0,0)旋转后:\n" << rotated << endl;

    // 缩放矩阵
    Vector3d scale;
    scale << 2, 3, 1;
    Matrix3d S = scale.asDiagonal();
    cout << "缩放矩阵:\n" << S << endl;

    // 组合变换
    Matrix3d T = R * S;
    cout << "旋转+缩放组合:\n" << T << endl;

    // ========== 12. 实用技巧 ==========
    cout << "\n--- 12. 实用技巧 ---" << endl;

    // 检查NaN和Inf
    MatrixXd test = MatrixXd::Random(3, 3);
    bool has_nan = !((test.array() == test.array()).all());
    bool has_inf = ((test.array().abs() == std::numeric_limits<double>::infinity()).any());
    if (has_nan) cout << "包含NaN" << endl;
    if (has_inf) cout << "包含Inf" << endl;
    cout << "矩阵检查通过" << endl;

    // 条件选择
    MatrixXd A_cond = MatrixXd::Random(3, 3);
    MatrixXd B_cond = MatrixXd::Random(3, 3);
    MatrixXd result = (A_cond.array() > B_cond.array()).select(A_cond, B_cond);
    cout << "条件选择（A>B选A，否则选B）" << endl;

    // 排序
    VectorXd unsorted = VectorXd::Random(5);
    cout << "未排序:\n" << unsorted << endl;
    std::sort(unsorted.data(), unsorted.data() + unsorted.size());
    cout << "排序后:\n" << unsorted << endl;

    // 查找最大最小值位置
    MatrixXd M_search = MatrixXd::Random(3, 3);
    int maxRow, maxCol, minRow, minCol;
    double maxVal = M_search.maxCoeff(&maxRow, &maxCol);
    double minVal = M_search.minCoeff(&minRow, &minCol);
    cout << "最大值 " << maxVal << " 在位置 (" << maxRow << "," << maxCol << ")" << endl;
    cout << "最小值 " << minVal << " 在位置 (" << minRow << "," << minCol << ")" << endl;

    cout << "\n=== Eigen综合用法示例完成 ===" << endl;
    cin.get();
    return 0;
}
