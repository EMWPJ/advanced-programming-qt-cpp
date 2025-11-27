#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵元素访问示例 ===" << endl;

    Matrix3d A;
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    cout << "矩阵A:\n" << A << endl;

    // ========== 单个元素访问 ==========
    cout << "\n1. 单个元素访问:" << endl;
    cout << "A(0, 0) = " << A(0, 0) << endl;
    cout << "A(1, 2) = " << A(1, 2) << endl;
    cout << "A(2, 1) = " << A(2, 1) << endl;

    // 修改元素
    A(0, 0) = 99;
    cout << "修改A(0,0) = 99后:\n" << A << endl;

    // ========== 行和列访问 ==========
    cout << "\n2. 行和列访问:" << endl;
    RowVector3d row0 = A.row(0);
    cout << "第0行: " << row0 << endl;

    Vector3d col0 = A.col(0);
    cout << "第0列:\n" << col0 << endl;

    // 修改行
    A.row(1) << 10, 11, 12;
    cout << "修改第1行后:\n" << A << endl;

    // 修改列
    A.col(2) << 13, 14, 15;
    cout << "修改第2列后:\n" << A << endl;

    // ========== 使用索引访问 ==========
    cout << "\n3. 使用索引访问:" << endl;
    MatrixXd B(3, 3);
    B << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    // 访问第i个元素（按列主序）
    cout << "B(0) = " << B(0) << " (第0行第0列)" << endl;
    cout << "B(3) = " << B(3) << " (第0行第1列)" << endl;
    cout << "B(6) = " << B(6) << " (第0行第2列)" << endl;

    // ========== 向量元素访问 ==========
    cout << "\n4. 向量元素访问:" << endl;
    VectorXd v(5);
    v << 1, 2, 3, 4, 5;
    cout << "向量v: " << v.transpose() << endl;

    cout << "v(0) = " << v(0) << endl;
    cout << "v[1] = " << v[1] << endl;  // 也可以使用[]
    cout << "v.tail(3) = " << v.tail(3).transpose() << endl;  // 后3个元素
    cout << "v.head(2) = " << v.head(2).transpose() << endl;  // 前2个元素

    // ========== 对角元素访问 ==========
    cout << "\n5. 对角元素访问:" << endl;
    Matrix3d C;
    C << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    Vector3d diagonal = C.diagonal();
    cout << "对角元素: " << diagonal.transpose() << endl;

    // 修改对角元素
    C.diagonal() << 10, 20, 30;
    cout << "修改对角元素后:\n" << C << endl;

    // ========== 使用指针访问 ==========
    cout << "\n6. 使用指针访问:" << endl;
    MatrixXd D(3, 3);
    D << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    double* data = D.data();
    cout << "使用data()指针访问:" << endl;
    for (int i = 0; i < 9; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;

    // ========== 边界检查 ==========
    cout << "\n7. 边界检查:" << endl;
    MatrixXd E(3, 3);
    E.setRandom();

    // 检查索引是否有效
    int row = 1, col = 2;
    if (row >= 0 && row < E.rows() && col >= 0 && col < E.cols()) {
        cout << "E(" << row << ", " << col << ") = " << E(row, col) << endl;
    }

    cin.get();
    return 0;
}
