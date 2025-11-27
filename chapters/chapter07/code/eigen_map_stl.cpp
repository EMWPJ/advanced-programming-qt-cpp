#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen Map类型与STL集成 ===" << endl;

    // ========== 1. Map类型：操作现有数据 ==========
    cout << "\n--- 1. Map类型：操作现有数据（避免拷贝）---" << endl;

    // 从C数组创建Map
    double data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Map<Matrix<double, 2, 3, RowMajor>> A_map(data);
    cout << "从C数组创建的Map矩阵:\n" << A_map << endl;

    // 修改原始数据会影响Map
    data[0] = 10.0;
    cout << "修改原始数据后:\n" << A_map << endl;

    // 从vector创建Map
    vector<double> vec_data = {1.0, 2.0, 3.0, 4.0};
    Map<VectorXd> v_map(vec_data.data(), vec_data.size());
    cout << "\n从vector创建的Map向量:\n" << v_map << endl;

    // 修改Map会影响原始vector
    v_map(0) = 100.0;
    cout << "修改Map后，原始vector: ["
         << vec_data[0] << ", " << vec_data[1] << ", "
         << vec_data[2] << ", " << vec_data[3] << "]" << endl;

    // ========== 2. Map的列主序和行主序 ==========
    cout << "\n--- 2. Map的列主序和行主序 ---" << endl;

    double matrix_data[] = {1, 2, 3, 4, 5, 6};

    // 列主序（默认）
    Map<Matrix<double, 2, 3>> A_colmajor(matrix_data);
    cout << "列主序Map (2x3):\n" << A_colmajor << endl;

    // 行主序
    Map<Matrix<double, 2, 3, RowMajor>> A_rowmajor(matrix_data);
    cout << "行主序Map (2x3):\n" << A_rowmajor << endl;

    // ========== 3. Map与Eigen矩阵的转换 ==========
    cout << "\n--- 3. Map与Eigen矩阵的转换 ---" << endl;

    MatrixXd A(3, 3);
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    // 将Eigen矩阵映射到Map（只读）
    Map<const MatrixXd> A_const_map(A.data(), A.rows(), A.cols());
    cout << "只读Map:\n" << A_const_map << endl;

    // 将Eigen矩阵数据复制到数组
    double* A_data = new double[A.size()];
    Map<MatrixXd> A_copy_map(A_data, A.rows(), A.cols());
    A_copy_map = A;  // 复制数据
    cout << "复制的Map:\n" << A_copy_map << endl;

    delete[] A_data;

    // ========== 4. 与STL vector集成 ==========
    cout << "\n--- 4. 与STL vector集成 ---" << endl;

    // 使用Eigen::aligned_allocator（推荐）
    vector<Vector3d, aligned_allocator<Vector3d>> vec_of_vectors;
    vec_of_vectors.push_back(Vector3d(1, 2, 3));
    vec_of_vectors.push_back(Vector3d(4, 5, 6));
    vec_of_vectors.push_back(Vector3d(7, 8, 9));

    cout << "vector中的向量:" << endl;
    for (const auto& v : vec_of_vectors) {
        cout << v.transpose() << endl;
    }

    // 将vector转换为矩阵（每列一个向量）
    MatrixXd vectors_matrix(3, vec_of_vectors.size());
    for (size_t i = 0; i < vec_of_vectors.size(); ++i) {
        vectors_matrix.col(i) = vec_of_vectors[i];
    }
    cout << "\n转换为矩阵:\n" << vectors_matrix << endl;

    // ========== 5. 从STL容器创建Eigen矩阵 ==========
    cout << "\n--- 5. 从STL容器创建Eigen矩阵 ---" << endl;

    // 从vector创建
    vector<double> stl_vec = {1.0, 2.0, 3.0, 4.0, 5.0};
    Map<VectorXd> eigen_vec(stl_vec.data(), stl_vec.size());
    cout << "从vector创建的Eigen向量:\n" << eigen_vec << endl;

    // 从array创建
    array<double, 6> stl_array = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Map<Matrix<double, 2, 3, RowMajor>> eigen_matrix(stl_array.data());
    cout << "\n从array创建的Eigen矩阵:\n" << eigen_matrix << endl;

    // ========== 6. 将Eigen矩阵转换为STL容器 ==========
    cout << "\n--- 6. 将Eigen矩阵转换为STL容器 ---" << endl;

    VectorXd eigen_vector(5);
    eigen_vector << 1, 2, 3, 4, 5;

    // 转换为vector
    vector<double> to_vector(eigen_vector.data(),
                             eigen_vector.data() + eigen_vector.size());
    cout << "转换为vector: [";
    for (size_t i = 0; i < to_vector.size(); ++i) {
        cout << to_vector[i];
        if (i < to_vector.size() - 1) cout << ", ";
    }
    cout << "]" << endl;

    // ========== 7. Map的高级用法：操作子矩阵 ==========
    cout << "\n--- 7. Map的高级用法：操作子矩阵 ---" << endl;

    double large_data[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    // 映射到3x4矩阵
    Map<Matrix<double, 3, 4, RowMajor>> large_matrix(large_data);
    cout << "大矩阵:\n" << large_matrix << endl;

    // 使用Map操作子块（需要小心内存布局）
    Map<Matrix<double, 2, 2, RowMajor>> sub_matrix(large_data + 5);
    cout << "\n子矩阵（从索引5开始）:\n" << sub_matrix << endl;

    // ========== 8. 实际应用：与C接口集成 ==========
    cout << "\n--- 8. 实际应用：与C接口集成 ---" << endl;

    // 模拟C函数返回的数据
    auto get_data = [](double* data, int size) {
        for (int i = 0; i < size; ++i) {
            data[i] = i + 1.0;
        }
    };

    double c_data[6];
    get_data(c_data, 6);

    Map<Matrix<double, 2, 3, RowMajor>> c_matrix(c_data);
    cout << "从C函数获取的数据:\n" << c_matrix << endl;

    // 修改后传回C函数
    c_matrix *= 2.0;
    cout << "修改后:\n" << c_matrix << endl;

    cin.get();
    return 0;
}
