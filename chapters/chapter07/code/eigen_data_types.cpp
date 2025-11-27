#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen基本数据类型示例 ===" << endl;

    // ========== 矩阵类型 ==========
    cout << "\n1. 矩阵类型:" << endl;

    // 动态大小矩阵
    MatrixXd dynamic_matrix(3, 3);  // 3x3动态大小矩阵
    dynamic_matrix << 1, 2, 3,
                      4, 5, 6,
                      7, 8, 9;
    cout << "MatrixXd (动态大小):\n" << dynamic_matrix << endl;

    // 固定大小矩阵
    Matrix3d fixed_matrix;  // 3x3固定大小矩阵
    fixed_matrix << 1, 0, 0,
                    0, 1, 0,
                    0, 0, 1;
    cout << "Matrix3d (固定大小):\n" << fixed_matrix << endl;

    // 不同标量类型
    MatrixXf float_matrix(2, 2);  // float类型
    float_matrix << 1.5f, 2.5f,
                    3.5f, 4.5f;
    cout << "MatrixXf (float类型):\n" << float_matrix << endl;

    Matrix3i int_matrix;  // int类型
    int_matrix << 1, 2, 3,
                   4, 5, 6,
                   7, 8, 9;
    cout << "Matrix3i (int类型):\n" << int_matrix << endl;

    // ========== 向量类型 ==========
    cout << "\n2. 向量类型:" << endl;

    // 列向量
    VectorXd col_vector(4);
    col_vector << 1, 2, 3, 4;
    cout << "VectorXd (列向量):\n" << col_vector << endl;

    // 行向量
    RowVectorXd row_vector(4);
    row_vector << 1, 2, 3, 4;
    cout << "RowVectorXd (行向量):\n" << row_vector << endl;

    // 固定大小向量
    Vector3d fixed_vector;
    fixed_vector << 1, 2, 3;
    cout << "Vector3d (固定大小):\n" << fixed_vector << endl;

    Vector4f float_vector;
    float_vector << 1.1f, 2.2f, 3.3f, 4.4f;
    cout << "Vector4f (float类型):\n" << float_vector << endl;

    // ========== Array类型 ==========
    cout << "\n3. Array类型 (元素级运算):" << endl;

    ArrayXd array1(4);
    array1 << 1, 2, 3, 4;
    ArrayXd array2(4);
    array2 << 5, 6, 7, 8;

    cout << "array1:\n" << array1 << endl;
    cout << "array2:\n" << array2 << endl;
    cout << "array1 * array2 (元素级乘法):\n" << array1 * array2 << endl;
    cout << "array1 / array2 (元素级除法):\n" << array1 / array2 << endl;

    // ========== 类型转换 ==========
    cout << "\n4. 类型转换:" << endl;

    MatrixXd matrix = MatrixXd::Random(3, 3);
    ArrayXXd array = matrix.array();  // Matrix转Array
    MatrixXd matrix2 = array.matrix();  // Array转Matrix

    cout << "Matrix:\n" << matrix << endl;
    cout << "转换为Array:\n" << array << endl;
    cout << "转换回Matrix:\n" << matrix2 << endl;

    // ========== 尺寸信息 ==========
    cout << "\n5. 尺寸信息:" << endl;

    MatrixXd m(5, 3);
    cout << "矩阵尺寸: " << m.rows() << " x " << m.cols() << endl;
    cout << "矩阵大小: " << m.size() << endl;
    cout << "是否为列向量: " << (m.IsVectorAtCompileTime ? "是" : "否") << endl;
    cout << "是否为行主序: " << (m.IsRowMajor ? "是" : "否") << endl;

    cin.get();
    return 0;
}
