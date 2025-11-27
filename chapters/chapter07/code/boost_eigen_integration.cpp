#include <iostream>
#include <Eigen/Dense>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/random.hpp>
#include <random>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace boost::multiprecision;
using namespace boost::math;
// 不使用 boost::random 命名空间，避免与 std::random 冲突

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost与Eigen结合示例 ===" << endl;

    // 示例1: 使用Boost高精度计算进行矩阵运算
    cout << "\n1. 高精度矩阵运算:" << endl;

    typedef cpp_dec_float_50 mp_float;

    // 创建高精度矩阵
    Matrix<mp_float, 3, 3> A_mp;
    A_mp << mp_float("1.23456789012345678901234567890"),
            mp_float("2.34567890123456789012345678901"),
            mp_float("3.45678901234567890123456789012"),
            mp_float("4.56789012345678901234567890123"),
            mp_float("5.67890123456789012345678901234"),
            mp_float("6.78901234567890123456789012345"),
            mp_float("7.89012345678901234567890123456"),
            mp_float("8.90123456789012345678901234567"),
            mp_float("9.01234567890123456789012345678");

    Matrix<mp_float, 3, 3> B_mp = A_mp.inverse();
    Matrix<mp_float, 3, 3> C_mp = A_mp * B_mp;

    cout << "高精度矩阵A:" << endl;
    cout << A_mp << endl;
    cout << "A的逆矩阵（高精度）:" << endl;
    cout << B_mp << endl;
    cout << "A * A^(-1)（应接近单位矩阵）:" << endl;
    cout << C_mp << endl;

    // 示例2: 使用Boost数学函数处理Eigen矩阵
    cout << "\n2. Boost数学函数与Eigen矩阵:" << endl;

    MatrixXd A = MatrixXd::Random(3, 3);
    A = A.array().abs();  // 确保值为正

    cout << "原始矩阵A:" << endl;
    cout << A << endl;

    // 对矩阵的每个元素应用Boost的数学函数
    MatrixXd A_gamma(3, 3);
    MatrixXd A_erf(3, 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            A_gamma(i, j) = tgamma(A(i, j));      // 伽马函数
            A_erf(i, j) = erf(A(i, j));            // 误差函数
        }
    }

    cout << "伽马函数 Γ(A):" << endl;
    cout << A_gamma << endl;
    cout << "误差函数 erf(A):" << endl;
    cout << A_erf << endl;

    // 示例3: 使用Boost随机数生成器填充Eigen矩阵
    cout << "\n3. Boost随机数生成Eigen矩阵:" << endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> normal_dist(0.0, 1.0);
    std::uniform_real_distribution<double> uniform_dist(-1.0, 1.0);

    MatrixXd random_normal(5, 5);
    MatrixXd random_uniform(5, 5);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            random_normal(i, j) = normal_dist(gen);
            random_uniform(i, j) = uniform_dist(gen);
        }
    }

    cout << "正态分布随机矩阵:" << endl;
    cout << random_normal << endl;
    cout << "均匀分布随机矩阵:" << endl;
    cout << random_uniform << endl;

    // 示例4: 高精度特征值计算
    cout << "\n4. 高精度特征值计算:" << endl;

    Matrix<mp_float, 2, 2> sym_matrix;
    sym_matrix << mp_float("4.0"), mp_float("2.0"),
                  mp_float("2.0"), mp_float("5.0");

    SelfAdjointEigenSolver<Matrix<mp_float, 2, 2>> eigen_solver(sym_matrix);
    Matrix<mp_float, 2, 1> eigenvalues = eigen_solver.eigenvalues();

    cout << "对称矩阵:" << endl;
    cout << sym_matrix << endl;
    cout << "特征值（高精度）:" << endl;
    cout << eigenvalues << endl;

    // 示例5: 使用Boost统计函数分析Eigen向量
    cout << "\n5. Boost统计函数分析Eigen向量:" << endl;

    VectorXd data = VectorXd::Random(100);

    // 计算统计量
    double mean = data.mean();
    double variance = (data.array() - mean).square().mean();
    double std_dev = sqrt(variance);

    cout << "数据向量统计:" << endl;
    cout << "均值: " << mean << endl;
    cout << "方差: " << variance << endl;
    cout << "标准差: " << std_dev << endl;

    // 使用Boost的正态分布
    boost::math::normal_distribution<double> data_dist(mean, std_dev);
    cout << "拟合的正态分布参数: N(" << mean << ", " << std_dev << ")" << endl;

    cin.get();
    return 0;
}
