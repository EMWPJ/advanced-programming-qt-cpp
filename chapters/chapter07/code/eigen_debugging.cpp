#include <iostream>
#include <cassert>
#include <limits>
#include <Eigen/Dense>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// 调试技巧示例：展示如何调试Eigen代码

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen调试技巧示例 ===" << endl;

    // 技巧1: 打印矩阵信息
    cout << "\n--- 技巧1: 打印矩阵信息 ---" << endl;

    MatrixXd A(3, 4);
    A << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12;

    cout << "矩阵A:" << endl;
    cout << "  行数: " << A.rows() << endl;
    cout << "  列数: " << A.cols() << endl;
    cout << "  大小: " << A.size() << endl;
    cout << "  是否为方阵: " << (A.rows() == A.cols() ? "是" : "否") << endl;
    cout << "  矩阵内容:" << endl << A << endl;

    // 技巧2: 使用断言检查维度
    cout << "\n--- 技巧2: 使用断言检查维度 ---" << endl;

    MatrixXd B(4, 3);
    B << 1, 2, 3,
         4, 5, 6,
         7, 8, 9,
         10, 11, 12;

    // 在Debug模式下，断言会检查维度
    assert(A.cols() == B.rows() && "矩阵维度不匹配！");

    MatrixXd C = A * B;
    cout << "矩阵乘法成功: A(" << A.rows() << "x" << A.cols()
         << ") * B(" << B.rows() << "x" << B.cols() << ") = C("
         << C.rows() << "x" << C.cols() << ")" << endl;

    // 技巧3: 检查NaN和Inf
    cout << "\n--- 技巧3: 检查NaN和Inf ---" << endl;

    MatrixXd test_matrix(3, 3);
    test_matrix << 1.0, 2.0, 3.0,
                   4.0, 5.0, 6.0,
                   7.0, 8.0, 9.0;

    // 检查输入
    bool has_nan = !((test_matrix.array() == test_matrix.array()).all());
    bool has_inf = ((test_matrix.array().abs() == std::numeric_limits<double>::infinity()).any());
    if (has_nan) {
        cerr << "错误: 输入矩阵包含NaN！" << endl;
        return 1;
    }
    if (has_inf) {
        cerr << "错误: 输入矩阵包含Inf！" << endl;
        return 1;
    }
    cout << "输入矩阵检查通过" << endl;

    // 执行运算
    MatrixXd result = test_matrix * test_matrix;

    // 检查结果
    bool result_has_nan = !((result.array() == result.array()).all());
    bool result_has_inf = ((result.array().abs() == std::numeric_limits<double>::infinity()).any());
    if (result_has_nan) {
        cerr << "错误: 结果包含NaN！" << endl;
    } else if (result_has_inf) {
        cerr << "错误: 结果包含Inf！" << endl;
    } else {
        cout << "结果检查通过" << endl;
    }

    // 技巧4: 逐步验证中间结果
    cout << "\n--- 技巧4: 逐步验证中间结果 ---" << endl;

    VectorXd x(3);
    x << 1.0, 2.0, 3.0;
    VectorXd b(3);
    b << 14.0, 32.0, 50.0;

    cout << "求解线性方程组 Ax = b" << endl;
    cout << "系数矩阵A:" << endl << test_matrix << endl;
    cout << "右端向量b:" << endl << b << endl;

    // 求解
    VectorXd solution = test_matrix.lu().solve(b);
    cout << "解向量x:" << endl << solution << endl;

    // 验证解
    VectorXd computed_b = test_matrix * solution;
    cout << "计算得到的b:" << endl << computed_b << endl;

    VectorXd residual = computed_b - b;
    double residual_norm = residual.norm();
    cout << "残差向量:" << endl << residual << endl;
    cout << "残差范数: " << residual_norm << endl;

    if (residual_norm < 1e-10) {
        cout << "解验证通过！" << endl;
    } else {
        cout << "警告: 残差较大，解可能不准确" << endl;
    }

    // 技巧5: 使用已知答案的测试用例
    cout << "\n--- 技巧5: 使用已知答案的测试用例 ---" << endl;

    // 创建一个简单的测试用例：单位矩阵
    Matrix3d identity = Matrix3d::Identity();
    Vector3d test_b;
    test_b << 1.0, 2.0, 3.0;

    Vector3d expected_x = test_b;  // 单位矩阵的解就是b本身
    Vector3d computed_x = identity.lu().solve(test_b);

    cout << "测试矩阵（单位矩阵）:" << endl << identity << endl;
    cout << "右端向量:" << endl << test_b << endl;
    cout << "期望解:" << endl << expected_x << endl;
    cout << "计算解:" << endl << computed_x << endl;

    double error = (computed_x - expected_x).norm();
    if (error < 1e-10) {
        cout << "测试通过！误差: " << error << endl;
    } else {
        cout << "测试失败！误差: " << error << endl;
    }

    // 技巧6: 对比不同算法的结果
    cout << "\n--- 技巧6: 对比不同算法的结果 ---" << endl;

    MatrixXd test_A = MatrixXd::Random(10, 10);
    test_A = test_A.transpose() * test_A;  // 对称正定
    VectorXd test_b2 = VectorXd::Random(10);

    // 使用不同的分解方法
    VectorXd x_lu = test_A.lu().solve(test_b2);
    VectorXd x_qr = test_A.householderQr().solve(test_b2);
    VectorXd x_chol = test_A.llt().solve(test_b2);

    // 计算残差
    double residual_lu = (test_A * x_lu - test_b2).norm();
    double residual_qr = (test_A * x_qr - test_b2).norm();
    double residual_chol = (test_A * x_chol - test_b2).norm();

    cout << "不同算法的残差对比:" << endl;
    cout << "  LU分解: " << residual_lu << endl;
    cout << "  QR分解: " << residual_qr << endl;
    cout << "  Cholesky分解: " << residual_chol << endl;

    // 检查解的差异
    double diff_lu_qr = (x_lu - x_qr).norm();
    double diff_lu_chol = (x_lu - x_chol).norm();

    cout << "\n解之间的差异:" << endl;
    cout << "  LU vs QR: " << diff_lu_qr << endl;
    cout << "  LU vs Cholesky: " << diff_lu_chol << endl;

    cout << "\n=== 调试技巧总结 ===" << endl;
    cout << "1. 打印矩阵的尺寸和内容" << endl;
    cout << "2. 使用断言检查维度匹配" << endl;
    cout << "3. 检查NaN和Inf值" << endl;
    cout << "4. 逐步验证中间结果" << endl;
    cout << "5. 使用已知答案的测试用例" << endl;
    cout << "6. 对比不同算法的结果" << endl;
    cout << "7. 计算残差验证解的正确性" << endl;
    cout << "8. 启用Eigen的边界检查（Debug模式）" << endl;

    cin.get();
    return 0;
}
