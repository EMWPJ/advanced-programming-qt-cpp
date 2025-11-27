#include <iostream>
#include <Eigen/Dense>
#include <windows.h>

using namespace std;
using namespace Eigen;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen矩阵块操作示例 ===" << endl;

    MatrixXd A(5, 5);
    A << 1, 2, 3, 4, 5,
         6, 7, 8, 9, 10,
         11, 12, 13, 14, 15,
         16, 17, 18, 19, 20,
         21, 22, 23, 24, 25;

    cout << "矩阵A:\n" << A << endl;

    // ========== 基本块操作 ==========
    cout << "\n1. 基本块操作:" << endl;

    // block(i, j, rows, cols) - 从(i,j)开始提取rows×cols块
    MatrixXd block1 = A.block(1, 1, 3, 3);
    cout << "A.block(1, 1, 3, 3):\n" << block1 << endl;

    // topLeftCorner(rows, cols) - 左上角
    MatrixXd topLeft = A.topLeftCorner(2, 2);
    cout << "A.topLeftCorner(2, 2):\n" << topLeft << endl;

    // topRightCorner(rows, cols) - 右上角
    MatrixXd topRight = A.topRightCorner(2, 2);
    cout << "A.topRightCorner(2, 2):\n" << topRight << endl;

    // bottomLeftCorner(rows, cols) - 左下角
    MatrixXd bottomLeft = A.bottomLeftCorner(2, 2);
    cout << "A.bottomLeftCorner(2, 2):\n" << bottomLeft << endl;

    // bottomRightCorner(rows, cols) - 右下角
    MatrixXd bottomRight = A.bottomRightCorner(2, 2);
    cout << "A.bottomRightCorner(2, 2):\n" << bottomRight << endl;

    // ========== 行和列块 ==========
    cout << "\n2. 行和列块:" << endl;

    // 中间行
    RowVectorXd middleRow = A.middleRows(2, 1);
    cout << "A.middleRows(2, 1):\n" << middleRow << endl;

    // 中间列
    VectorXd middleCol = A.middleCols(2, 1);
    cout << "A.middleCols(2, 1):\n" << middleCol << endl;

    // 前n行
    MatrixXd topRows = A.topRows(2);
    cout << "A.topRows(2):\n" << topRows << endl;

    // 后n行
    MatrixXd bottomRows = A.bottomRows(2);
    cout << "A.bottomRows(2):\n" << bottomRows << endl;

    // 前n列
    MatrixXd leftCols = A.leftCols(2);
    cout << "A.leftCols(2):\n" << leftCols << endl;

    // 后n列
    MatrixXd rightCols = A.rightCols(2);
    cout << "A.rightCols(2):\n" << rightCols << endl;

    // ========== 修改块 ==========
    cout << "\n3. 修改块:" << endl;

    MatrixXd B = A;
    B.block(1, 1, 2, 2) << 99, 99,
                            99, 99;
    cout << "修改块后:\n" << B << endl;

    // 使用块赋值
    MatrixXd C = A;
    Matrix2d replacement;
    replacement << 100, 101,
                   102, 103;
    C.topLeftCorner(2, 2) = replacement;
    cout << "使用块赋值:\n" << C << endl;

    // ========== 向量块操作 ==========
    cout << "\n4. 向量块操作:" << endl;

    VectorXd v(10);
    v << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
    cout << "向量v: " << v.transpose() << endl;

    // head(n) - 前n个元素
    VectorXd head = v.head(3);
    cout << "v.head(3): " << head.transpose() << endl;

    // tail(n) - 后n个元素
    VectorXd tail = v.tail(3);
    cout << "v.tail(3): " << tail.transpose() << endl;

    // segment(i, n) - 从位置i开始的n个元素
    VectorXd segment = v.segment(2, 4);
    cout << "v.segment(2, 4): " << segment.transpose() << endl;

    // ========== 对角块 ==========
    cout << "\n5. 对角块:" << endl;

    MatrixXd D(4, 4);
    D.setRandom();
    cout << "矩阵D:\n" << D << endl;

    VectorXd diagonal = D.diagonal();
    cout << "对角元素: " << diagonal.transpose() << endl;

    // 修改对角元素
    D.diagonal() << 1, 2, 3, 4;
    cout << "修改对角元素后:\n" << D << endl;

    // ========== 块操作性能 ==========
    cout << "\n6. 块操作性能提示:" << endl;
    cout << "block()返回引用，不复制数据" << endl;
    cout << "如果需要修改，使用block()直接赋值" << endl;
    cout << "如果需要副本，使用auto或显式类型" << endl;

    // 引用（不复制）
    auto& blockRef = A.block(1, 1, 2, 2);
    blockRef(0, 0) = 999;
    cout << "使用引用修改后A:\n" << A << endl;

    cin.get();
    return 0;
}
