#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵创建与初始化示例 ===" << endl;

    // ========== 方法1: 使用逗号初始化 ==========
    cout << "\n1. 逗号初始化:" << endl;

    Matrix3d m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    cout << "m1:\n" << m1 << endl;

    Vector3d v1;
    v1 << 1, 2, 3;
    cout << "v1:\n" << v1 << endl;

    // ========== 方法2: 构造函数初始化 ==========
    cout << "\n2. 构造函数初始化:" << endl;

    MatrixXd m2(3, 3);  // 未初始化
    m2.setZero();  // 设置为零矩阵
    cout << "m2 (零矩阵):\n" << m2 << endl;

    MatrixXd m3 = MatrixXd::Zero(3, 3);  // 直接创建零矩阵
    cout << "m3 (零矩阵):\n" << m3 << endl;

    MatrixXd m4 = MatrixXd::Ones(3, 3);  // 全1矩阵
    cout << "m4 (全1矩阵):\n" << m4 << endl;

    MatrixXd m5 = MatrixXd::Identity(3, 3);  // 单位矩阵
    cout << "m5 (单位矩阵):\n" << m5 << endl;

    MatrixXd m6 = MatrixXd::Random(3, 3);  // 随机矩阵
    cout << "m6 (随机矩阵):\n" << m6 << endl;

    MatrixXd m7 = MatrixXd::Constant(3, 3, 2.5);  // 常数矩阵
    cout << "m7 (常数矩阵):\n" << m7 << endl;

    // ========== 方法3: 从数组初始化 ==========
    cout << "\n3. 从数组初始化:" << endl;

    double data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Map<Matrix3d> m8(data);  // 映射数组到矩阵
    cout << "m8 (从数组):\n" << m8 << endl;

    // ========== 方法4: 块操作创建 ==========
    cout << "\n4. 块操作创建:" << endl;

    MatrixXd m9(5, 5);
    m9.setRandom();
    cout << "m9:\n" << m9 << endl;

    MatrixXd block = m9.block(1, 1, 3, 3);  // 提取3x3块
    cout << "提取块 (1,1) 开始的3x3:\n" << block << endl;

    // ========== 方法5: 特殊矩阵 ==========
    cout << "\n5. 特殊矩阵:" << endl;

    // 对角矩阵
    Vector3d diag_values;
    diag_values << 1, 2, 3;
    Matrix3d diag_matrix = diag_values.asDiagonal();
    cout << "对角矩阵:\n" << diag_matrix << endl;

    // 从向量创建矩阵
    VectorXd vec(9);
    vec << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    MatrixXd m10 = Map<MatrixXd>(vec.data(), 3, 3);
    cout << "从向量重塑为矩阵:\n" << m10 << endl;

    // ========== 方法6: 复制和赋值 ==========
    cout << "\n6. 复制和赋值:" << endl;

    Matrix3d m11;
    m11 << 1, 0, 0,
           0, 1, 0,
           0, 0, 1;

    Matrix3d m12 = m11;  // 深拷贝
    m12(0, 0) = 99;
    cout << "m11 (原矩阵):\n" << m11 << endl;
    cout << "m12 (修改后的副本):\n" << m12 << endl;

    // ========== 方法7: 动态调整大小 ==========
    cout << "\n7. 动态调整大小:" << endl;

    MatrixXd m13(2, 2);
    m13 << 1, 2,
           3, 4;
    cout << "原始大小 (2x2):\n" << m13 << endl;

    m13.resize(3, 3);
    m13.setZero();
    m13(0, 0) = 1; m13(1, 1) = 1; m13(2, 2) = 1;
    cout << "调整后大小 (3x3):\n" << m13 << endl;

    // ========== 方法8: 向量初始化 ==========
    cout << "\n8. 向量初始化:" << endl;

    VectorXd v2 = VectorXd::LinSpaced(5, 0, 4);  // 线性空间
    cout << "LinSpaced(5, 0, 4):\n" << v2 << endl;

    VectorXd v3 = VectorXd::Random(5);  // 随机向量
    cout << "Random(5):\n" << v3 << endl;

    VectorXd v4 = VectorXd::Ones(5);  // 全1向量
    cout << "Ones(5):\n" << v4 << endl;

    VectorXd v5 = VectorXd::Zero(5);  // 零向量
    cout << "Zero(5):\n" << v5 << endl;

    cin.get();
    return 0;
}
