#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#include <windows.h>

using namespace std;
using namespace Eigen;

// ========== 应用1: 坐标变换 ==========
void coordinate_transform_example() {
    cout << "=== 应用1: 坐标变换 ===" << endl;

    // 定义3D点
    Vector3d point(1.0, 2.0, 3.0);
    cout << "原始点: [" << point.transpose() << "]" << endl;

    // 绕Z轴旋转90度
    AngleAxisd rotation_z(M_PI/2, Vector3d::UnitZ());

    // 平移
    Translation3d translation(1.0, 2.0, 3.0);

    // 组合变换：先旋转后平移
    Transform<double, 3, Affine> transform = translation * rotation_z;

    Vector3d transformed_point = transform * point;
    cout << "变换后的点: [" << transformed_point.transpose() << "]" << endl;

    // 2D坐标变换示例
    Vector2d point_2d(1.0, 2.0);
    Rotation2D<double> rotation_2d(M_PI/4);  // 旋转45度
    Vector2d rotated_2d = rotation_2d * point_2d;
    cout << "\n2D点旋转45度: [" << rotated_2d.transpose() << "]" << endl;
}

// ========== 应用2: 线性回归 ==========
void linear_regression_example() {
    cout << "\n=== 应用2: 线性回归 ===" << endl;

    // 生成样本数据 y = 2*x + 1 + noise
    int n_samples = 10;
    MatrixXd X(n_samples, 2);  // [1, x]
    VectorXd y(n_samples);

    for (int i = 0; i < n_samples; ++i) {
        double x_val = i * 0.5;
        X(i, 0) = 1.0;  // 偏置项
        X(i, 1) = x_val;
        y(i) = 2.0 * x_val + 1.0 + 0.1 * (i % 3 - 1);  // 添加噪声
    }

    // 最小二乘求解: (X^T * X) * theta = X^T * y
    VectorXd theta = (X.transpose() * X).ldlt().solve(X.transpose() * y);

    cout << "真实参数: [1.0, 2.0]" << endl;
    cout << "估计参数: [" << theta(0) << ", " << theta(1) << "]" << endl;
    cout << "回归方程: y = " << theta(1) << "*x + " << theta(0) << endl;
}

// ========== 应用3: 曲线拟合（多项式拟合）==========
void curve_fitting_example() {
    cout << "\n=== 应用3: 曲线拟合（多项式拟合）===" << endl;

    // 生成数据点 y = x^2 - 2*x + 1 + noise
    int n_points = 10;
    VectorXd x_data(n_points);
    VectorXd y_data(n_points);

    for (int i = 0; i < n_points; ++i) {
        double x = i * 0.3;
        x_data(i) = x;
        y_data(i) = x*x - 2*x + 1 + 0.1 * (i % 3 - 1);
    }

    // 构造Vandermonde矩阵（二次多项式）
    int degree = 2;
    MatrixXd A(n_points, degree + 1);
    for (int i = 0; i < n_points; ++i) {
        for (int j = 0; j <= degree; ++j) {
            A(i, j) = pow(x_data(i), j);
        }
    }

    // 最小二乘求解
    VectorXd coeffs = A.householderQr().solve(y_data);

    cout << "真实系数: [1, -2, 1]" << endl;
    cout << "拟合系数: [" << coeffs(0) << ", " << coeffs(1)
         << ", " << coeffs(2) << "]" << endl;
    cout << "拟合多项式: y = " << coeffs(2) << "*x^2 + "
         << coeffs(1) << "*x + " << coeffs(0) << endl;
}

// ========== 应用4: PCA主成分分析 ==========
void pca_example() {
    cout << "\n=== 应用4: PCA主成分分析 ===" << endl;

    // 生成二维数据（带相关性）
    int n_samples = 20;
    MatrixXd data(2, n_samples);

    // 生成椭圆分布的数据
    for (int i = 0; i < n_samples; ++i) {
        double angle = 2 * M_PI * i / n_samples;
        data(0, i) = 3 * cos(angle) + 0.3 * (i % 3 - 1);
        data(1, i) = 1 * sin(angle) + 0.3 * (i % 3 - 1);
    }

    // 中心化
    Vector2d mean = data.rowwise().mean();
    MatrixXd centered = data.colwise() - mean;

    // 计算协方差矩阵
    Matrix2d cov = (centered * centered.transpose()) / (n_samples - 1);

    // 特征值分解
    SelfAdjointEigenSolver<Matrix2d> eigen_solver(cov);
    Vector2d eigenvalues = eigen_solver.eigenvalues();
    Matrix2d eigenvectors = eigen_solver.eigenvectors();

    cout << "协方差矩阵:\n" << cov << endl;
    cout << "特征值: [" << eigenvalues.transpose() << "]" << endl;
    cout << "主成分（特征向量）:\n" << eigenvectors << endl;

    // 投影到第一主成分
    Vector2d first_component = eigenvectors.col(1);  // 最大特征值对应的特征向量
    VectorXd projection = first_component.transpose() * centered;
    cout << "投影到第一主成分后的数据维度: " << projection.size() << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen实际应用案例（简化版）===" << endl;

    coordinate_transform_example();
    linear_regression_example();
    curve_fitting_example();
    pca_example();

    cin.get();
    return 0;
}
