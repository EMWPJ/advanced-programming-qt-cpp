#include <iostream>
#include <vector>
#include <chrono>
#include <Eigen/Dense>

// 使用MKL加速Eigen
#define EIGEN_USE_MKL_ALL
#include <mkl.h>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <random>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;
using namespace boost::multiprecision;
using namespace boost::math;
// 不使用 boost::random 命名空间，避免与 std::random 冲突
using namespace boost::accumulators;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== 综合应用示例：多库协同 ===" << endl;

    // 场景：科学计算中的完整工作流
    // 1. 使用Boost生成高精度随机数据
    // 2. 使用Eigen进行矩阵运算（MKL加速）
    // 3. 使用Boost进行统计分析
    // 4. 使用Boost高精度计算验证结果

    cout << "\n=== 步骤1: 生成高精度随机数据 ===" << endl;

    typedef cpp_dec_float_50 mp_float;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> normal_dist(0.0, 1.0);

    const int n = 50;
    MatrixXd data_matrix(n, n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            data_matrix(i, j) = normal_dist(gen);
        }
    }

    cout << "生成了 " << n << "x" << n << " 的随机数据矩阵" << endl;

    cout << "\n=== 步骤2: 使用Eigen+MKL进行矩阵分解 ===" << endl;

    MatrixXd A = data_matrix.transpose() * data_matrix;  // 对称正定矩阵

    auto start = high_resolution_clock::now();

    // 使用MKL加速的Cholesky分解
    LLT<MatrixXd> chol(A);
    MatrixXd L = chol.matrixL();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Cholesky分解完成（MKL加速）" << endl;
    cout << "耗时: " << duration.count() << " 微秒" << endl;

    // 验证分解正确性
    MatrixXd reconstructed = L * L.transpose();
    double error = (A - reconstructed).norm();
    cout << "重构误差: " << error << endl;

    cout << "\n=== 步骤3: 使用Boost进行统计分析 ===" << endl;

    // 提取矩阵的对角元素进行统计分析
    VectorXd diagonal = A.diagonal();

    accumulator_set<double, stats<tag::mean, tag::variance, tag::min, tag::max>> acc;

    for (int i = 0; i < diagonal.size(); ++i) {
        acc(diagonal(i));
    }

    cout << "对角元素统计:" << endl;
    cout << "  均值: " << mean(acc) << endl;
    cout << "  方差: " << variance(acc) << endl;
    double min_val = boost::accumulators::min(acc);
    double max_val = boost::accumulators::max(acc);
    cout << "  最小值: " << min_val << endl;
    cout << "  最大值: " << max_val << endl;

    cout << "\n=== 步骤4: 高精度验证计算 ===" << endl;

    // 使用高精度计算验证小矩阵的运算
    Matrix<mp_float, 3, 3> test_matrix;
    test_matrix << mp_float("1.0"), mp_float("2.0"), mp_float("3.0"),
                   mp_float("2.0"), mp_float("5.0"), mp_float("7.0"),
                   mp_float("3.0"), mp_float("7.0"), mp_float("11.0");

    Matrix<mp_float, 3, 3> test_inv = test_matrix.inverse();
    Matrix<mp_float, 3, 3> test_identity = test_matrix * test_inv;

    cout << "测试矩阵（高精度）:" << endl;
    cout << test_matrix << endl;
    cout << "逆矩阵（高精度）:" << endl;
    cout << test_inv << endl;
    cout << "验证 A * A^(-1):" << endl;
    cout << test_identity << endl;

    cout << "\n=== 步骤5: 综合应用 - 求解优化问题 ===" << endl;

    // 使用Eigen求解线性最小二乘问题
    // min ||Ax - b||^2
    MatrixXd A_ls = MatrixXd::Random(100, 20);
    VectorXd b_ls = VectorXd::Random(100);

    start = high_resolution_clock::now();

    // 使用QR分解求解（MKL加速）
    VectorXd x_ls = A_ls.householderQr().solve(b_ls);

    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    cout << "最小二乘问题求解完成（MKL加速）" << endl;
    cout << "耗时: " << duration.count() << " 微秒" << endl;

    // 计算残差
    VectorXd residual = A_ls * x_ls - b_ls;
    double residual_norm = residual.norm();
    cout << "残差范数: " << residual_norm << endl;

    // 使用Boost统计函数分析残差分布
    accumulator_set<double, stats<tag::mean, tag::variance>> residual_acc;
    for (int i = 0; i < residual.size(); ++i) {
        residual_acc(residual(i));
    }

    cout << "残差统计:" << endl;
    cout << "  均值: " << mean(residual_acc) << endl;
    cout << "  标准差: " << sqrt(variance(residual_acc)) << endl;

    cout << "\n=== 步骤6: 使用Boost特殊函数处理特征值 ===" << endl;

    // 计算矩阵的特征值
    MatrixXd sym_A = A_ls.transpose() * A_ls;
    SelfAdjointEigenSolver<MatrixXd> eigen_solver(sym_A);
    VectorXd eigenvals = eigen_solver.eigenvalues();

    cout << "特征值范围: [" << eigenvals.minCoeff()
         << ", " << eigenvals.maxCoeff() << "]" << endl;

    // 使用Boost的数学函数处理特征值
    cout << "对特征值应用数学函数:" << endl;
    for (int i = 0; i < (std::min)(5, (int)eigenvals.size()); ++i) {
        double val = eigenvals(i);
        if (val > 0) {
            cout << "  λ[" << i << "] = " << val << endl;
            cout << "    ln(λ) = " << log(val) << endl;
            cout << "    sqrt(λ) = " << sqrt(val) << endl;
            if (val < 10) {
                cout << "    Γ(λ) = " << tgamma(val) << endl;
            }
        }
    }

    cout << "\n=== 完成：多库协同计算示例 ===" << endl;
    cout << "展示了Eigen、MKL、Boost的协同使用" << endl;

    cin.get();
    return 0;
}
