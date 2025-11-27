#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;

// Eigen表达式模板和延迟求值示例

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen表达式模板和延迟求值 ===" << endl;

    // ========== 1. 表达式模板基础 ==========
    cout << "\n--- 1. 表达式模板基础 ---" << endl;

    MatrixXd A = MatrixXd::Random(3, 3);
    MatrixXd B = MatrixXd::Random(3, 3);
    MatrixXd C = MatrixXd::Random(3, 3);

    cout << "矩阵A:\n" << A << endl;
    cout << "矩阵B:\n" << B << endl;

    // 表达式模板：不会立即计算，而是存储表达式
    auto expr = A + B;  // 这是一个表达式对象，不是矩阵

    // 只有在赋值时才会计算
    MatrixXd D = expr;  // 此时才计算 A + B
    cout << "A + B:\n" << D << endl;

    // ========== 2. 延迟求值的优势 ==========
    cout << "\n--- 2. 延迟求值的优势 ---" << endl;

    // 复杂表达式只计算一次
    MatrixXd result1 = A + B + C;  // 优化为 (A+B)+C，只创建一次临时对象

    // 对比：如果没有表达式模板，会创建多个临时对象
    MatrixXd temp1 = A + B;
    MatrixXd temp2 = temp1 + C;

    cout << "表达式模板结果:\n" << result1 << endl;
    cout << "手动计算结果:\n" << temp2 << endl;

    // ========== 3. 避免不必要的临时对象 ==========
    cout << "\n--- 3. 避免不必要的临时对象 ---" << endl;

    MatrixXd E = MatrixXd::Random(100, 100);
    MatrixXd F = MatrixXd::Random(100, 100);
    MatrixXd G = MatrixXd::Random(100, 100);
    MatrixXd H = MatrixXd::Random(100, 100);

    // 不好的写法：会创建临时对象
    auto start1 = high_resolution_clock::now();
    MatrixXd bad_result = E * F + G * H;
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);

    // 好的写法：使用noalias避免临时对象
    MatrixXd good_result(100, 100);
    auto start2 = high_resolution_clock::now();
    good_result.noalias() = E * F + G * H;
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);

    cout << "不使用noalias: " << duration1.count() << " 微秒" << endl;
    cout << "使用noalias: " << duration2.count() << " 微秒" << endl;

    // ========== 4. 表达式模板的陷阱 ==========
    cout << "\n--- 4. 表达式模板的陷阱 ---" << endl;

    MatrixXd X = MatrixXd::Random(3, 3);
    MatrixXd Y = MatrixXd::Random(3, 3);

    // 陷阱1：自赋值问题
    cout << "原始X:\n" << X << endl;

    // 错误：X = X + Y 会导致问题（因为X在计算时已经被修改）
    // 正确做法：使用临时变量或eval()
    MatrixXd X_old = X;
    X = X_old + Y;  // 正确
    cout << "X = X + Y:\n" << X << endl;

    // 或者使用eval()
    X = (X + Y).eval();  // eval()强制立即求值

    // ========== 5. eval()的使用 ==========
    cout << "\n--- 5. eval()的使用 ---" << endl;

    MatrixXd M1 = MatrixXd::Random(3, 3);
    MatrixXd M2 = MatrixXd::Random(3, 3);

    // 需要复制表达式结果时使用eval()
    MatrixXd M3 = (M1 + M2).eval();  // 显式求值并复制

    // 转置后需要复制
    MatrixXd M4 = M1.transpose().eval();  // 转置并复制

    cout << "M1:\n" << M1 << endl;
    cout << "M1的转置（复制）:\n" << M4 << endl;

    // ========== 6. 表达式模板与块操作 ==========
    cout << "\n--- 6. 表达式模板与块操作 ---" << endl;

    MatrixXd M = MatrixXd::Random(5, 5);
    MatrixXd N = MatrixXd::Random(5, 5);

    // 块操作也支持表达式模板
    M.block(0, 0, 3, 3) = N.block(0, 0, 3, 3) + MatrixXd::Identity(3, 3);

    cout << "M的前3x3块:\n" << M.block(0, 0, 3, 3) << endl;

    // ========== 7. 复杂表达式的优化 ==========
    cout << "\n--- 7. 复杂表达式的优化 ---" << endl;

    VectorXd v1 = VectorXd::Random(100);
    VectorXd v2 = VectorXd::Random(100);
    VectorXd v3 = VectorXd::Random(100);

    // 复杂表达式：Eigen会自动优化
    double dot_result = v1.dot(v2) + v2.dot(v3) + v3.dot(v1);
    cout << "复杂点积表达式结果: " << dot_result << endl;

    // 矩阵表达式
    MatrixXd P = MatrixXd::Random(10, 10);
    MatrixXd Q = MatrixXd::Random(10, 10);

    // 表达式：P^T * Q * P（Eigen会优化计算顺序）
    MatrixXd R = P.transpose() * Q * P;
    cout << "P^T * Q * P 的大小: " << R.rows() << "x" << R.cols() << endl;

    // ========== 8. 性能对比 ==========
    cout << "\n--- 8. 性能对比 ---" << endl;

    const int size = 500;
    MatrixXd A_large = MatrixXd::Random(size, size);
    MatrixXd B_large = MatrixXd::Random(size, size);
    MatrixXd C_large = MatrixXd::Random(size, size);

    // 方法1：直接计算（会创建临时对象）
    auto t1 = high_resolution_clock::now();
    MatrixXd result_method1 = A_large * B_large + C_large;
    auto t2 = high_resolution_clock::now();

    // 方法2：使用noalias
    MatrixXd result_method2(size, size);
    auto t3 = high_resolution_clock::now();
    result_method2.noalias() = A_large * B_large + C_large;
    auto t4 = high_resolution_clock::now();

    // 方法3：分步计算
    MatrixXd result_method3(size, size);
    auto t5 = high_resolution_clock::now();
    result_method3 = A_large * B_large;
    result_method3 += C_large;
    auto t6 = high_resolution_clock::now();

    auto time1 = duration_cast<milliseconds>(t2 - t1).count();
    auto time2 = duration_cast<milliseconds>(t4 - t3).count();
    auto time3 = duration_cast<milliseconds>(t6 - t5).count();

    cout << "方法1（直接计算）: " << time1 << " ms" << endl;
    cout << "方法2（noalias）: " << time2 << " ms" << endl;
    cout << "方法3（分步计算）: " << time3 << " ms" << endl;

    cin.get();
    return 0;
}
