#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#include <vector>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// ========== 应用1: 线性回归 ==========
void linear_regression_example() {
    cout << "\n=== 应用1: 线性回归 ===" << endl;

    // 生成样本数据 y = 2*x + 1 + noise
    int n_samples = 20;
    MatrixXd X(n_samples, 2);  // [1, x]
    VectorXd y(n_samples);

    for (int i = 0; i < n_samples; ++i) {
        double x_val = i * 0.5;
        X(i, 0) = 1.0;  // 偏置项
        X(i, 1) = x_val;
        y(i) = 2.0 * x_val + 1.0 + 0.1 * (rand() % 10 - 5);  // 添加噪声
    }

    // 最小二乘求解: (X^T * X) * theta = X^T * y
    VectorXd theta = (X.transpose() * X).ldlt().solve(X.transpose() * y);

    cout << "真实参数: [1.0, 2.0]" << endl;
    cout << "估计参数: [" << theta(0) << ", " << theta(1) << "]" << endl;
    cout << "回归方程: y = " << theta(1) << "*x + " << theta(0) << endl;
}

// ========== 应用2: 多项式拟合 ==========
void polynomial_fitting_example() {
    cout << "\n=== 应用2: 多项式拟合 ===" << endl;

    // 生成数据点 y = x^2 - 2*x + 1 + noise
    int n_points = 15;
    VectorXd x_data(n_points);
    VectorXd y_data(n_points);

    for (int i = 0; i < n_points; ++i) {
        double x = i * 0.3;
        x_data(i) = x;
        y_data(i) = x*x - 2*x + 1 + 0.1 * (rand() % 10 - 5);
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

// ========== 应用3: 图像旋转 ==========
void image_rotation_example() {
    cout << "\n=== 应用3: 图像旋转（2D点变换）===" << endl;

    // 定义几个2D点
    MatrixXd points(2, 4);
    points << 0, 1, 1, 0,
               0, 0, 1, 1;  // 正方形四个顶点

    cout << "原始点:\n" << points << endl;

    // 旋转45度
    double angle = M_PI / 4.0;
    Rotation2D<double> rotation(angle);
    Matrix2d R = rotation.toRotationMatrix();

    // 应用旋转变换
    MatrixXd rotated_points = R * points;

    cout << "旋转45度后的点:\n" << rotated_points << endl;

    // 组合变换：旋转 + 平移
    Transform<double, 2, Affine> transform = Translation2d(2.0, 1.0) * rotation;
    MatrixXd transformed_points = transform * points.colwise().homogeneous();

    cout << "旋转+平移后的点:\n" << transformed_points << endl;
}

// ========== 应用4: 3D坐标变换 ==========
void coordinate_transform_example() {
    cout << "\n=== 应用4: 3D坐标变换 ===" << endl;

    // 定义3D点
    Vector3d point(1.0, 2.0, 3.0);
    cout << "原始点: [" << point.transpose() << "]" << endl;

    // 绕Z轴旋转90度
    AngleAxisd rotation_z(M_PI/2, Vector3d::UnitZ());

    // 平移
    Translation3d translation(1.0, 2.0, 3.0);

    // 组合变换
    Transform<double, 3, Affine> transform = translation * rotation_z;

    Vector3d transformed_point = transform * point;
    cout << "变换后的点: [" << transformed_point.transpose() << "]" << endl;
}

// ========== 应用5: 主成分分析（PCA） ==========
void pca_example() {
    cout << "\n=== 应用5: 主成分分析（PCA）===" << endl;

    // 生成二维数据（带相关性）
    int n_samples = 100;
    MatrixXd data(2, n_samples);

    // 生成椭圆分布的数据
    for (int i = 0; i < n_samples; ++i) {
        double angle = 2 * M_PI * i / n_samples;
        data(0, i) = 3 * cos(angle) + 0.5 * (rand() % 10 - 5);
        data(1, i) = 1 * sin(angle) + 0.5 * (rand() % 10 - 5);
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

    // 投影到主成分空间
    int n_components = 1;
    MatrixXd projection = eigenvectors.leftCols(n_components).transpose() * centered;
    cout << "投影到第一主成分后的数据维度: " << projection.rows()
         << " x " << projection.cols() << endl;
}

// ========== 应用6: 弹簧-质量系统 ==========
void spring_mass_system_example() {
    cout << "\n=== 应用6: 弹簧-质量系统（动力学）===" << endl;

    // 三个质量块，用弹簧连接
    // 系统方程: M * x'' + K * x = F
    // 简化为: K * x = F (静力学)

    int n_masses = 3;
    MatrixXd K(n_masses, n_masses);  // 刚度矩阵
    VectorXd F(n_masses);             // 外力向量

    // 构造刚度矩阵（三对角）
    double k = 100.0;  // 弹簧刚度
    K << 2*k, -k, 0,
         -k, 2*k, -k,
         0, -k, k;

    // 外力（只在第一个质量上）
    F << 10.0, 0.0, 0.0;

    cout << "刚度矩阵K:\n" << K << endl;
    cout << "外力向量F:\n" << F << endl;

    // 求解位移
    VectorXd displacement = K.ldlt().solve(F);

    cout << "位移向量x:\n" << displacement << endl;
    cout << "最大位移: " << displacement.maxCoeff() << " m" << endl;
}

// ========== 应用7: 最小二乘曲线拟合 ==========
void curve_fitting_example() {
    cout << "\n=== 应用7: 最小二乘曲线拟合 ===" << endl;

    // 拟合函数: y = a*exp(b*x) + c
    // 线性化: ln(y-c) = ln(a) + b*x
    // 使用非线性最小二乘的线性近似

    int n_points = 20;
    VectorXd x(n_points);
    VectorXd y(n_points);

    // 生成数据
    double a_true = 2.0, b_true = -0.5, c_true = 1.0;
    for (int i = 0; i < n_points; ++i) {
        x(i) = i * 0.2;
        y(i) = a_true * exp(b_true * x(i)) + c_true + 0.1 * (rand() % 10 - 5);
    }

    // 简化：假设c已知，线性化
    double c = 1.0;
    VectorXd y_adj = (y.array() - c).max(0.01);  // 避免负数
    VectorXd log_y = y_adj.array().log();

    // 构造线性系统
    MatrixXd A(n_points, 2);
    A.col(0) = VectorXd::Ones(n_points);
    A.col(1) = x;

    // 求解
    VectorXd params = A.householderQr().solve(log_y);
    double a_est = exp(params(0));
    double b_est = params(1);

    cout << "真实参数: a=" << a_true << ", b=" << b_true << ", c=" << c_true << endl;
    cout << "估计参数: a=" << a_est << ", b=" << b_est << ", c=" << c << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen实际应用案例集合 ===" << endl;

    linear_regression_example();
    polynomial_fitting_example();
    image_rotation_example();
    coordinate_transform_example();
    pca_example();
    spring_mass_system_example();
    curve_fitting_example();

    cin.get();
    return 0;
}
