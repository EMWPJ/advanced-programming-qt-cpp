#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// Eigen稀疏矩阵完整示例

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen稀疏矩阵完整示例 ===" << endl;

    // ========== 1. 创建稀疏矩阵 ==========
    cout << "\n--- 1. 创建稀疏矩阵 ---" << endl;

    const int rows = 5, cols = 5;
    SparseMatrix<double> sparse_mat(rows, cols);

    // 预分配非零元素数量
    sparse_mat.reserve(10);

    // 插入非零元素
    sparse_mat.insert(0, 0) = 1.0;
    sparse_mat.insert(0, 1) = 2.0;
    sparse_mat.insert(1, 1) = 3.0;
    sparse_mat.insert(1, 2) = 4.0;
    sparse_mat.insert(2, 2) = 5.0;
    sparse_mat.insert(2, 3) = 6.0;
    sparse_mat.insert(3, 3) = 7.0;
    sparse_mat.insert(3, 4) = 8.0;
    sparse_mat.insert(4, 4) = 9.0;

    // 压缩存储格式
    sparse_mat.makeCompressed();

    cout << "稀疏矩阵非零元素数: " << sparse_mat.nonZeros() << endl;
    cout << "稀疏矩阵:\n" << MatrixXd(sparse_mat) << endl;

    // ========== 2. 从稠密矩阵创建稀疏矩阵 ==========
    cout << "\n--- 2. 从稠密矩阵创建稀疏矩阵 ---" << endl;

    MatrixXd dense_mat(4, 4);
    dense_mat << 1, 0, 0, 2,
                 0, 3, 0, 0,
                 0, 0, 4, 0,
                 5, 0, 0, 6;

    cout << "稠密矩阵:\n" << dense_mat << endl;

    SparseMatrix<double> sparse_from_dense = dense_mat.sparseView();
    cout << "转换为稀疏矩阵，非零元素数: " << sparse_from_dense.nonZeros() << endl;
    cout << "稀疏矩阵:\n" << MatrixXd(sparse_from_dense) << endl;

    // ========== 3. 稀疏矩阵运算 ==========
    cout << "\n--- 3. 稀疏矩阵运算 ---" << endl;

    SparseMatrix<double> A(3, 3);
    A.reserve(5);
    A.insert(0, 0) = 2.0;
    A.insert(0, 1) = 1.0;
    A.insert(1, 1) = 3.0;
    A.insert(1, 2) = 1.0;
    A.insert(2, 2) = 4.0;
    A.makeCompressed();

    VectorXd x(3);
    x << 1, 2, 3;

    cout << "稀疏矩阵A:\n" << MatrixXd(A) << endl;
    cout << "向量x:\n" << x << endl;

    // 矩阵-向量乘法
    VectorXd y = A * x;
    cout << "A * x:\n" << y << endl;

    // 稀疏矩阵转置
    SparseMatrix<double> A_T = A.transpose();
    cout << "A转置:\n" << MatrixXd(A_T) << endl;

    // 稀疏矩阵加法
    SparseMatrix<double> B(3, 3);
    B.reserve(3);
    B.insert(0, 0) = 1.0;
    B.insert(1, 1) = 1.0;
    B.insert(2, 2) = 1.0;
    B.makeCompressed();

    SparseMatrix<double> C = A + B;
    cout << "A + B:\n" << MatrixXd(C) << endl;

    // ========== 4. 稀疏矩阵分解 ==========
    cout << "\n--- 4. 稀疏矩阵分解 ---" << endl;

    // 创建对称正定稀疏矩阵
    SparseMatrix<double> sym_mat(4, 4);
    sym_mat.reserve(10);
    sym_mat.insert(0, 0) = 4.0;
    sym_mat.insert(0, 1) = 1.0;
    sym_mat.insert(1, 0) = 1.0;
    sym_mat.insert(1, 1) = 3.0;
    sym_mat.insert(1, 2) = 1.0;
    sym_mat.insert(2, 1) = 1.0;
    sym_mat.insert(2, 2) = 2.0;
    sym_mat.insert(2, 3) = 1.0;
    sym_mat.insert(3, 2) = 1.0;
    sym_mat.insert(3, 3) = 5.0;
    sym_mat.makeCompressed();

    cout << "对称稀疏矩阵:\n" << MatrixXd(sym_mat) << endl;

    // SimplicialLLT分解（Cholesky）
    SimplicialLLT<SparseMatrix<double>> llt(sym_mat);
    if (llt.info() != Success) {
        cout << "分解失败" << endl;
    } else {
        cout << "SimplicialLLT分解成功" << endl;

        VectorXd b(4);
        b << 1, 2, 3, 4;
        VectorXd x_solve = llt.solve(b);
        cout << "求解 Ax = b，解x:\n" << x_solve << endl;

        // 验证
        VectorXd b_computed = sym_mat * x_solve;
        cout << "验证 Ax:\n" << b_computed << endl;
    }

    // ========== 5. 稀疏矩阵迭代器 ==========
    cout << "\n--- 5. 稀疏矩阵迭代器 ---" << endl;

    cout << "遍历稀疏矩阵非零元素:" << endl;
    for (int k = 0; k < sparse_mat.outerSize(); ++k) {
        for (SparseMatrix<double>::InnerIterator it(sparse_mat, k); it; ++it) {
            cout << "(" << it.row() << "," << it.col() << ") = "
                 << it.value() << endl;
        }
    }

    // ========== 6. 稀疏矩阵特殊构造 ==========
    cout << "\n--- 6. 稀疏矩阵特殊构造 ---" << endl;

    // 创建对角稀疏矩阵
    VectorXd diag_vals(5);
    diag_vals << 1, 2, 3, 4, 5;
    SparseMatrix<double> diag_sparse(5, 5);
    for (int i = 0; i < 5; ++i) {
        diag_sparse.insert(i, i) = diag_vals(i);
    }
    diag_sparse.makeCompressed();
    cout << "对角稀疏矩阵:\n" << MatrixXd(diag_sparse) << endl;

    // 创建三对角矩阵
    SparseMatrix<double> tridiag(5, 5);
    tridiag.reserve(13);  // 5 + 4 + 4 = 13
    for (int i = 0; i < 5; ++i) {
        if (i > 0) tridiag.insert(i, i-1) = -1.0;
        tridiag.insert(i, i) = 2.0;
        if (i < 4) tridiag.insert(i, i+1) = -1.0;
    }
    tridiag.makeCompressed();
    cout << "三对角稀疏矩阵:\n" << MatrixXd(tridiag) << endl;

    // ========== 7. 稀疏矩阵性能对比 ==========
    cout << "\n--- 7. 稀疏矩阵性能对比 ---" << endl;

    const int large_size = 1000;
    const double sparsity = 0.01;  // 1%非零元素

    // 创建大型稀疏矩阵
    SparseMatrix<double> large_sparse(large_size, large_size);
    int nnz = static_cast<int>(large_size * large_size * sparsity);
    large_sparse.reserve(nnz);

    srand(42);
    for (int i = 0; i < nnz; ++i) {
        int row = rand() % large_size;
        int col = rand() % large_size;
        // 使用coeffRef避免重复插入同一位置的错误
        large_sparse.coeffRef(row, col) = static_cast<double>(rand()) / RAND_MAX;
    }
    large_sparse.makeCompressed();

    cout << "大型稀疏矩阵 (" << large_size << "x" << large_size << ")" << endl;
    cout << "非零元素数: " << large_sparse.nonZeros() << endl;
    cout << "稀疏度: " << (1.0 - double(large_sparse.nonZeros()) /
                           (large_size * large_size)) * 100 << "%" << endl;

    // 矩阵-向量乘法
    VectorXd large_vec = VectorXd::Random(large_size);
    VectorXd result = large_sparse * large_vec;
    cout << "矩阵-向量乘法完成，结果向量大小: " << result.size() << endl;

    // ========== 8. 稀疏矩阵应用：图邻接矩阵 ==========
    cout << "\n--- 8. 稀疏矩阵应用：图邻接矩阵 ---" << endl;

    // 创建图的邻接矩阵（无向图）
    SparseMatrix<double> graph(5, 5);
    graph.reserve(10);

    // 边: 0-1, 1-2, 2-3, 3-4, 0-4
    graph.insert(0, 1) = 1.0;
    graph.insert(1, 0) = 1.0;
    graph.insert(1, 2) = 1.0;
    graph.insert(2, 1) = 1.0;
    graph.insert(2, 3) = 1.0;
    graph.insert(3, 2) = 1.0;
    graph.insert(3, 4) = 1.0;
    graph.insert(4, 3) = 1.0;
    graph.insert(0, 4) = 1.0;
    graph.insert(4, 0) = 1.0;
    graph.makeCompressed();

    cout << "图邻接矩阵:\n" << MatrixXd(graph) << endl;

    // 计算每个节点的度（连接数）
    VectorXd degrees = graph * VectorXd::Ones(5);
    cout << "节点度数:\n" << degrees << endl;

    cout << "\n=== 稀疏矩阵示例完成 ===" << endl;
    cin.get();
    return 0;
}
