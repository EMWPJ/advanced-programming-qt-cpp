#include <iostream>
#include <vector>
#include <chrono>
#include <Eigen/Dense>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;
using namespace chrono;

// 性能对比示例：展示优化技巧

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen性能优化示例 ===" << endl;

    const int size = 500;
    const int iterations = 10;

    // 创建测试矩阵
    MatrixXd A = MatrixXd::Random(size, size);
    MatrixXd B = MatrixXd::Random(size, size);
    MatrixXd C(size, size);

    cout << "\n矩阵大小: " << size << "x" << size << endl;
    cout << "迭代次数: " << iterations << endl;

    // 测试1: 不好的做法 - 创建临时对象
    cout << "\n--- 测试1: 临时对象（不推荐）---" << endl;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        C = A * B + A;  // 创建临时对象
    }
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);
    cout << "耗时: " << duration1.count() << " 毫秒" << endl;

    // 测试2: 好的做法 - 使用noalias避免临时对象
    cout << "\n--- 测试2: 使用noalias（推荐）---" << endl;
    start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        C.noalias() = A * B;  // 避免临时对象
        C.noalias() += A;
    }
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);
    cout << "耗时: " << duration2.count() << " 毫秒" << endl;
    cout << "性能提升: " << (double)duration1.count() / duration2.count()
         << "x" << endl;

    // 测试3: 固定大小矩阵 vs 动态矩阵
    cout << "\n--- 测试3: 固定大小矩阵（小矩阵）---" << endl;
    Matrix3d fixed_A = Matrix3d::Random();
    Matrix3d fixed_B = Matrix3d::Random();
    Matrix3d fixed_C;

    start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        fixed_C = fixed_A * fixed_B;  // 栈上分配，无动态分配
    }
    end = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(end - start);
    cout << "固定大小矩阵耗时: " << duration3.count() << " 微秒" << endl;

    MatrixXd dynamic_A = MatrixXd::Random(3, 3);
    MatrixXd dynamic_B = MatrixXd::Random(3, 3);
    MatrixXd dynamic_C(3, 3);

    start = high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        dynamic_C = dynamic_A * dynamic_B;  // 堆上分配
    }
    end = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(end - start);
    cout << "动态矩阵耗时: " << duration4.count() << " 微秒" << endl;
    cout << "性能提升: " << (double)duration4.count() / duration3.count()
         << "x" << endl;

    // 测试4: 使用Map避免数据拷贝
    cout << "\n--- 测试4: 使用Map操作现有数据 ---" << endl;
    vector<double> data(size * size);
    for (int i = 0; i < size * size; ++i) {
        data[i] = i * 0.001;
    }

    // 使用Map直接操作vector数据，无需拷贝
    Map<MatrixXd> mapped_matrix(data.data(), size, size);
    MatrixXd result = mapped_matrix * mapped_matrix;
    cout << "使用Map完成矩阵乘法，无需数据拷贝" << endl;

    // 测试5: 批量操作 vs 循环
    cout << "\n--- 测试5: 批量操作 vs 循环 ---" << endl;
    VectorXd v1 = VectorXd::Random(size);
    VectorXd v2 = VectorXd::Random(size);
    VectorXd v3(size);

    // 循环方式
    start = high_resolution_clock::now();
    for (int i = 0; i < size; ++i) {
        v3(i) = v1(i) * v2(i) + v1(i);
    }
    end = high_resolution_clock::now();
    auto duration5 = duration_cast<microseconds>(end - start);
    cout << "循环方式耗时: " << duration5.count() << " 微秒" << endl;

    // 批量操作
    start = high_resolution_clock::now();
    v3 = v1.cwiseProduct(v2) + v1;  // 向量化操作
    end = high_resolution_clock::now();
    auto duration6 = duration_cast<microseconds>(end - start);
    cout << "批量操作耗时: " << duration6.count() << " 微秒" << endl;
    cout << "性能提升: " << (double)duration5.count() / duration6.count()
         << "x" << endl;

    cout << "\n=== 性能优化总结 ===" << endl;
    cout << "1. 使用noalias()避免不必要的临时对象" << endl;
    cout << "2. 小矩阵使用固定大小类型（栈分配）" << endl;
    cout << "3. 使用Map操作现有数据，避免拷贝" << endl;
    cout << "4. 优先使用批量操作而非循环" << endl;
    cout << "5. 启用编译器优化选项（-O3 -march=native）" << endl;

    cin.get();
    return 0;
}
