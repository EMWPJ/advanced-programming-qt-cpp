#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <memory>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// Eigen最佳实践和常见陷阱

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen最佳实践和常见陷阱 ===" << endl;

    // ========== 1. 固定大小 vs 动态大小 ==========
    cout << "\n--- 1. 固定大小 vs 动态大小 ---" << endl;

    // 固定大小矩阵（编译时已知尺寸）- 推荐用于小矩阵
    Matrix3d fixed_matrix;  // 栈上分配，零开销
    fixed_matrix << 1, 2, 3,
                     4, 5, 6,
                     7, 8, 9;

    // 动态大小矩阵（运行时确定尺寸）- 用于大矩阵或尺寸未知
    MatrixXd dynamic_matrix(3, 3);  // 堆上分配
    dynamic_matrix = fixed_matrix;

    cout << "固定大小矩阵:\n" << fixed_matrix << endl;
    cout << "动态大小矩阵:\n" << dynamic_matrix << endl;

    // 性能建议：小矩阵（<16x16）使用固定大小

    // ========== 2. 避免在循环中频繁分配 ==========
    cout << "\n--- 2. 避免在循环中频繁分配 ---" << endl;

    // 不好的做法：每次循环都分配
    cout << "不好的做法（在循环中分配）:" << endl;
    for (int i = 0; i < 3; ++i) {
        MatrixXd temp(100, 100);  // 每次循环都分配
        temp = MatrixXd::Random(100, 100);
    }

    // 好的做法：在循环外预分配
    cout << "好的做法（预分配）:" << endl;
    MatrixXd preallocated(100, 100);
    for (int i = 0; i < 3; ++i) {
        preallocated = MatrixXd::Random(100, 100);  // 重用已分配的内存
    }

    // ========== 3. 使用noalias避免临时对象 ==========
    cout << "\n--- 3. 使用noalias避免临时对象 ---" << endl;

    MatrixXd A = MatrixXd::Random(50, 50);
    MatrixXd B = MatrixXd::Random(50, 50);
    MatrixXd C = MatrixXd::Random(50, 50);

    // 不好的做法：会创建临时对象
    MatrixXd D_bad = A * B + C;

    // 好的做法：使用noalias
    MatrixXd D_good(50, 50);
    D_good.noalias() = A * B + C;

    cout << "使用noalias可以避免不必要的临时对象" << endl;

    // ========== 4. 自赋值问题 ==========
    cout << "\n--- 4. 自赋值问题 ---" << endl;

    MatrixXd M = MatrixXd::Random(3, 3);
    MatrixXd N = MatrixXd::Random(3, 3);

    cout << "原始M:\n" << M << endl;

    // 错误：M = M + N 会导致问题
    // 正确：先保存或使用eval()
    MatrixXd M_copy = M;
    M = M_copy + N;  // 正确

    // 或者
    M = (M + N).eval();  // 正确

    cout << "修改后M:\n" << M << endl;

    // ========== 5. 转置的陷阱 ==========
    cout << "\n--- 5. 转置的陷阱 ---" << endl;

    MatrixXd P = MatrixXd::Random(3, 3);

    // 错误：不能直接赋值转置给自己
    // P = P.transpose();  // 错误！

    // 正确：使用eval()或临时变量
    P = P.transpose().eval();  // 正确

    // 或者使用inPlace转置（仅适用于方阵）
    P.transposeInPlace();  // 正确，原地转置

    cout << "转置后P:\n" << P << endl;

    // ========== 6. 内存对齐 ==========
    cout << "\n--- 6. 内存对齐（固定大小矩阵自动对齐）---" << endl;

    // 固定大小矩阵自动对齐（SIMD优化）
    Matrix4d aligned_matrix;  // 自动16字节对齐
    aligned_matrix = Matrix4d::Random();

    // 动态矩阵需要手动对齐（使用aligned_allocator）
    // 在STL容器中使用Eigen类型时
    vector<Vector4d, aligned_allocator<Vector4d>> aligned_vectors;

    cout << "固定大小矩阵自动对齐，支持SIMD优化" << endl;

    // ========== 7. 使用Map避免拷贝 ==========
    cout << "\n--- 7. 使用Map避免拷贝 ---" << endl;

    vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    // 不好的做法：拷贝数据
    MatrixXd copied_matrix(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            copied_matrix(i, j) = data[i * 3 + j];
        }
    }

    // 好的做法：使用Map（零拷贝）
    Map<Matrix<double, 2, 3, RowMajor>> mapped_matrix(data.data());

    cout << "使用Map避免数据拷贝:\n" << mapped_matrix << endl;

    // ========== 8. 智能指针管理大矩阵 ==========
    cout << "\n--- 8. 智能指针管理大矩阵 ---" << endl;

    // 使用unique_ptr管理大矩阵
    auto large_matrix = make_unique<MatrixXd>(1000, 1000);
    *large_matrix = MatrixXd::Random(1000, 1000);

    // 自动释放内存
    cout << "使用智能指针管理大矩阵，自动释放内存" << endl;

    // ========== 9. 条件编译优化 ==========
    cout << "\n--- 9. 条件编译优化 ---" << endl;

    // 在Release模式下禁用边界检查
    #ifndef EIGEN_NO_DEBUG
        cout << "Debug模式：启用边界检查" << endl;
    #else
        cout << "Release模式：禁用边界检查（更快）" << endl;
    #endif

    // ========== 10. 错误处理 ==========
    cout << "\n--- 10. 错误处理 ---" << endl;

    MatrixXd singular_matrix(3, 3);
    singular_matrix << 1, 2, 3,
                        2, 4, 6,
                        3, 6, 9;  // 奇异矩阵（行列式为0）

    // 检查矩阵是否可逆
    if (abs(singular_matrix.determinant()) < 1e-10) {
        cout << "警告：矩阵接近奇异，行列式 = "
             << singular_matrix.determinant() << endl;
    }

    // 使用分解方法检查
    PartialPivLU<MatrixXd> lu(singular_matrix);
    // 注意：PartialPivLU没有info()方法，可以通过检查行列式来判断
    if (abs(singular_matrix.determinant()) < 1e-10) {
        cout << "LU分解失败：矩阵奇异" << endl;
    }

    cin.get();
    return 0;
}
