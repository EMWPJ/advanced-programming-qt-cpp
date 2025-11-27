#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

// Intel MKL 稀疏BLAS头文件
#include <mkl.h>
#include <mkl_spblas.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_matrix(const char* name, const double* matrix, int rows, int cols, int max_print = 4) {
    cout << name << " (" << rows << "x" << cols << "):" << endl;
    int print_rows = min(rows, max_print);
    int print_cols = min(cols, max_print);
    for (int i = 0; i < print_rows; ++i) {
        for (int j = 0; j < print_cols; ++j) {
            cout << setw(8) << fixed << setprecision(2) << matrix[i * cols + j] << "\t";
        }
        if (cols > print_cols) cout << "...";
        cout << endl;
    }
    if (rows > print_rows) cout << "...\n";
    cout << endl;
}

void print_sparse_csr(const char* name, int m, int n, const int* row_ptr,
                      const int* col_idx, const double* values, int max_print = 10) {
    cout << name << " (CSR格式, " << m << "x" << n << "):" << endl;
    cout << "非零元素数量: " << row_ptr[m] << endl;

    int print_count = 0;
    for (int i = 0; i < m && print_count < max_print; ++i) {
        for (int j = row_ptr[i]; j < row_ptr[i+1] && print_count < max_print; ++j) {
            cout << "A[" << i << "," << col_idx[j] << "] = " << values[j] << endl;
            print_count++;
        }
    }
    if (row_ptr[m] > max_print) {
        cout << "... (共 " << row_ptr[m] << " 个非零元素)" << endl;
    }
    cout << endl;
}

void print_sparse_coo(const char* name, int m, int n, int nnz,
                      const int* row_idx, const int* col_idx, const double* values, int max_print = 10) {
    cout << name << " (COO格式, " << m << "x" << n << "):" << endl;
    cout << "非零元素数量: " << nnz << endl;

    int print_count = min(nnz, max_print);
    for (int i = 0; i < print_count; ++i) {
        cout << "A[" << row_idx[i] << "," << col_idx[i] << "] = " << values[i] << endl;
    }
    if (nnz > max_print) {
        cout << "... (共 " << nnz << " 个非零元素)" << endl;
    }
    cout << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 稀疏BLAS示例 ===" << endl;

    // 示例1: 稀疏矩阵-向量乘法 (SpMV) - CSR格式
    cout << "\n=== 1. 稀疏矩阵-向量乘法 (SpMV) - CSR格式 ===" << endl;
    {
        const int m = 5, n = 5;

        // 创建一个简单的稀疏矩阵（三对角矩阵）
        // 矩阵:
        // [2  1  0  0  0]
        // [1  2  1  0  0]
        // [0  1  2  1  0]
        // [0  0  1  2  1]
        // [0  0  0  1  2]

        vector<int> row_ptr = {0, 2, 5, 8, 11, 13};  // CSR行指针
        vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4};  // 列索引
        vector<double> values = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};

        vector<double> x(n, 1.0);  // 输入向量
        vector<double> y(m, 0.0);  // 输出向量

        cout << "输入向量 x:" << endl;
        for (int i = 0; i < n; ++i) {
            cout << x[i] << " ";
        }
        cout << endl << endl;

        // 执行稀疏矩阵-向量乘法: y = A * x
        // 首先创建稀疏矩阵句柄
        sparse_matrix_t A;
        vector<int> row_start(m);
        vector<int> row_end(m);
        for (int i = 0; i < m; ++i) {
            row_start[i] = row_ptr[i];
            row_end[i] = row_ptr[i + 1];
        }
        sparse_status_t status = mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO,
                                                          m, n, row_start.data(), row_end.data(),
                                                          col_idx.data(), values.data());
        if (status != SPARSE_STATUS_SUCCESS) {
            cout << "错误：无法创建稀疏矩阵" << endl;
            return 1;
        }

        // 创建矩阵描述符
        struct matrix_descr descrA;
        descrA.type = SPARSE_MATRIX_TYPE_GENERAL;
        descrA.mode = SPARSE_FILL_MODE_FULL;
        descrA.diag = SPARSE_DIAG_NON_UNIT;

        double alpha = 1.0, beta = 0.0;

        auto start = high_resolution_clock::now();
        status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A, descrA,
                                 x.data(), beta, y.data());
        mkl_sparse_destroy(A);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "输出向量 y = A * x:" << endl;
        for (int i = 0; i < m; ++i) {
            cout << y[i] << " ";
        }
        cout << endl;
        cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 示例2: 使用稀疏矩阵描述符
    cout << "\n=== 2. 使用稀疏矩阵描述符 ===" << endl;
    {
        const int m = 4, n = 4;

        // 创建一个4x4稀疏矩阵
        vector<int> row_ptr = {0, 2, 4, 6, 7};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 3};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

        sparse_matrix_t A;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;
        descr.mode = SPARSE_FILL_MODE_FULL;
        descr.diag = SPARSE_DIAG_NON_UNIT;

        // 创建稀疏矩阵描述符
        int status = mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                           row_ptr.data(), row_ptr.data() + 1,
                                           col_idx.data(), values.data());

        if (status != SPARSE_STATUS_SUCCESS) {
            cout << "创建稀疏矩阵描述符失败" << endl;
            return 1;
        }

        vector<double> x(n, 1.0);
        vector<double> y(m, 0.0);

        // 执行矩阵-向量乘法
        double alpha = 1.0, beta = 0.0;
        status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A, descr,
                                x.data(), beta, y.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "使用描述符的矩阵-向量乘法结果:" << endl;
            for (int i = 0; i < m; ++i) {
                cout << y[i] << " ";
            }
            cout << endl << endl;
        }

        // 释放描述符
        mkl_sparse_destroy(A);
    }

    // 示例3: 稀疏矩阵-矩阵乘法 (SpMM)
    cout << "\n=== 3. 稀疏矩阵-矩阵乘法 (SpMM) ===" << endl;
    {
        const int m = 3, n = 3, k = 3;

        // 创建稀疏矩阵A (3x3)
        vector<int> row_ptr_A = {0, 2, 4, 6};
        vector<int> col_idx_A = {0, 1, 1, 2, 0, 2};
        vector<double> values_A = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        // 创建密集矩阵B (3x3)
        vector<double> B = {1.0, 2.0, 3.0,
                           4.0, 5.0, 6.0,
                           7.0, 8.0, 9.0};

        // 结果矩阵C (3x3)
        vector<double> C(m * k, 0.0);

        sparse_matrix_t A;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        int status = mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                           row_ptr_A.data(), row_ptr_A.data() + 1,
                                           col_idx_A.data(), values_A.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            double alpha = 1.0, beta = 0.0;

            // C = A * B
            status = mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A, descr,
                                     SPARSE_LAYOUT_ROW_MAJOR, B.data(), k, k,
                                     beta, C.data(), k);

            if (status == SPARSE_STATUS_SUCCESS) {
                cout << "矩阵B:" << endl;
                print_matrix("B", B.data(), k, k, 3);

                cout << "结果矩阵C = A * B:" << endl;
                print_matrix("C", C.data(), m, k, 3);
            }
        }

        mkl_sparse_destroy(A);
    }

    // 示例4: 稀疏矩阵加法
    cout << "\n=== 4. 稀疏矩阵加法 ===" << endl;
    {
        const int m = 3, n = 3;

        // 矩阵A
        vector<int> row_ptr_A = {0, 2, 4, 5};
        vector<int> col_idx_A = {0, 1, 1, 2, 0};
        vector<double> values_A = {1.0, 2.0, 3.0, 4.0, 5.0};

        // 矩阵B
        vector<int> row_ptr_B = {0, 1, 3, 4};
        vector<int> col_idx_B = {0, 1, 2, 2};
        vector<double> values_B = {6.0, 7.0, 8.0, 9.0};

        sparse_matrix_t A, B, C;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                row_ptr_A.data(), row_ptr_A.data() + 1,
                                col_idx_A.data(), values_A.data());

        mkl_sparse_d_create_csr(&B, SPARSE_INDEX_BASE_ZERO, m, n,
                                row_ptr_B.data(), row_ptr_B.data() + 1,
                                col_idx_B.data(), values_B.data());

        // C = A + B
        double alpha = 1.0, beta = 1.0;
        int status = mkl_sparse_d_add(SPARSE_OPERATION_NON_TRANSPOSE, A, alpha, B, &C);

        if (status == SPARSE_STATUS_SUCCESS) {
            // 获取结果矩阵的信息
            sparse_index_base_t indexing;
            int rows, cols;
            int* rows_start, *rows_end, *col_idx;
            double* values;

            mkl_sparse_d_export_csr(C, &indexing, &rows, &cols, &rows_start, &rows_end, &col_idx, &values);

            cout << "矩阵A + B (CSR格式):" << endl;
            print_sparse_csr("C", rows, cols, rows_start, col_idx, values, 10);
        }

        mkl_sparse_destroy(A);
        mkl_sparse_destroy(B);
        mkl_sparse_destroy(C);
    }

    // 示例5: 稀疏矩阵转置
    cout << "\n=== 5. 稀疏矩阵转置 ===" << endl;
    {
        const int m = 3, n = 4;

        vector<int> row_ptr = {0, 2, 4, 6};
        vector<int> col_idx = {0, 2, 1, 3, 0, 2};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        sparse_matrix_t A, AT;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                               row_ptr.data(), row_ptr.data() + 1,
                               col_idx.data(), values.data());

        // 创建转置矩阵
        int status = mkl_sparse_convert_csr(A, SPARSE_OPERATION_TRANSPOSE, &AT);

        if (status == SPARSE_STATUS_SUCCESS) {
            sparse_index_base_t indexing;
            int rows, cols;
            int* rows_start, *rows_end, *col_idx_T;
            double* values_T;

            mkl_sparse_d_export_csr(AT, &indexing, &rows, &cols, &rows_start, &rows_end, &col_idx_T, &values_T);

            cout << "原矩阵A (" << m << "x" << n << "):" << endl;
            print_sparse_csr("A", m, n, row_ptr.data(), col_idx.data(), values.data(), 10);

            cout << "转置矩阵A^T (" << rows << "x" << cols << "):" << endl;
            print_sparse_csr("A^T", rows, cols, rows_start, col_idx_T, values_T, 10);
        }

        mkl_sparse_destroy(A);
        mkl_sparse_destroy(AT);
    }

    // 示例6: COO格式创建
    cout << "\n=== 6. COO格式创建稀疏矩阵 ===" << endl;
    {
        const int m = 4, n = 4;
        const int nnz = 6;

        // COO格式：坐标格式 (row, col, value)
        vector<int> row_idx = {0, 0, 1, 1, 2, 3};
        vector<int> col_idx = {0, 2, 1, 3, 2, 3};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        sparse_matrix_t A;
        sparse_status_t status = mkl_sparse_d_create_coo(&A, SPARSE_INDEX_BASE_ZERO,
                                                          m, n, nnz,
                                                          row_idx.data(), col_idx.data(), values.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "COO格式矩阵创建成功" << endl;
            print_sparse_coo("A (COO)", m, n, nnz, row_idx.data(), col_idx.data(), values.data(), 10);

            // 转换为CSR格式
            sparse_matrix_t A_csr;
            status = mkl_sparse_convert_csr(A, SPARSE_OPERATION_NON_TRANSPOSE, &A_csr);
            if (status == SPARSE_STATUS_SUCCESS) {
                sparse_index_base_t indexing;
                int rows, cols;
                int* rows_start, *rows_end, *col_idx_csr;
                double* values_csr;

                mkl_sparse_d_export_csr(A_csr, &indexing, &rows, &cols, &rows_start, &rows_end, &col_idx_csr, &values_csr);
                cout << "转换为CSR格式:" << endl;
                print_sparse_csr("A (CSR)", rows, cols, rows_start, col_idx_csr, values_csr, 10);
                mkl_sparse_destroy(A_csr);
            }

            // 测试矩阵-向量乘法
            vector<double> x(n, 1.0);
            vector<double> y(m, 0.0);
            struct matrix_descr descr;
            descr.type = SPARSE_MATRIX_TYPE_GENERAL;

            status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, descr,
                                     x.data(), 0.0, y.data());
            if (status == SPARSE_STATUS_SUCCESS) {
                cout << "COO格式矩阵-向量乘法结果:" << endl;
                for (int i = 0; i < m; ++i) {
                    cout << y[i] << " ";
                }
                cout << endl << endl;
            }
        }

        mkl_sparse_destroy(A);
    }

    // 示例7: CSC格式创建
    cout << "\n=== 7. CSC格式创建稀疏矩阵 ===" << endl;
    {
        const int m = 4, n = 4;

        // CSC格式：列指针、行索引、值
        vector<int> col_ptr = {0, 2, 4, 6, 7};
        vector<int> row_idx = {0, 2, 1, 3, 0, 2, 3};
        vector<double> values = {1.0, 3.0, 2.0, 4.0, 5.0, 6.0, 7.0};

        sparse_matrix_t A;
        vector<int> col_start(n);
        vector<int> col_end(n);
        for (int j = 0; j < n; ++j) {
            col_start[j] = col_ptr[j];
            col_end[j] = col_ptr[j + 1];
        }

        sparse_status_t status = mkl_sparse_d_create_csc(&A, SPARSE_INDEX_BASE_ZERO,
                                                          m, n,
                                                          col_start.data(), col_end.data(),
                                                          row_idx.data(), values.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "CSC格式矩阵创建成功" << endl;
            cout << "列指针: ";
            for (int j = 0; j <= n; ++j) {
                cout << col_ptr[j] << " ";
            }
            cout << endl;
            cout << "行索引: ";
            for (int i = 0; i < col_ptr[n]; ++i) {
                cout << row_idx[i] << " ";
            }
            cout << endl;
            cout << "值: ";
            for (int i = 0; i < col_ptr[n]; ++i) {
                cout << values[i] << " ";
            }
            cout << endl << endl;

            // 测试矩阵-向量乘法
            vector<double> x(n, 1.0);
            vector<double> y(m, 0.0);
            struct matrix_descr descr;
            descr.type = SPARSE_MATRIX_TYPE_GENERAL;

            status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, descr,
                                     x.data(), 0.0, y.data());
            if (status == SPARSE_STATUS_SUCCESS) {
                cout << "CSC格式矩阵-向量乘法结果:" << endl;
                for (int i = 0; i < m; ++i) {
                    cout << y[i] << " ";
                }
                cout << endl << endl;
            }
        }

        mkl_sparse_destroy(A);
    }

    // 示例8: 对称矩阵和矩阵描述符
    cout << "\n=== 8. 对称矩阵和矩阵描述符 ===" << endl;
    {
        const int m = 4, n = 4;

        // 创建对称矩阵（下三角部分）
        vector<int> row_ptr = {0, 1, 3, 5, 7};
        vector<int> col_idx = {0, 0, 1, 0, 2, 1, 3};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

        sparse_matrix_t A;
        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                row_ptr.data(), row_ptr.data() + 1,
                                col_idx.data(), values.data());

        // 使用对称矩阵描述符
        struct matrix_descr descr_sym;
        descr_sym.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
        descr_sym.mode = SPARSE_FILL_MODE_LOWER;
        descr_sym.diag = SPARSE_DIAG_NON_UNIT;

        vector<double> x(n, 1.0);
        vector<double> y(m, 0.0);

        sparse_status_t status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, descr_sym,
                                                 x.data(), 0.0, y.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "对称矩阵（下三角）描述符测试:" << endl;
            cout << "矩阵类型: SYMMETRIC" << endl;
            cout << "填充模式: LOWER" << endl;
            cout << "矩阵-向量乘法结果:" << endl;
            for (int i = 0; i < m; ++i) {
                cout << y[i] << " ";
            }
            cout << endl << endl;
        }

        mkl_sparse_destroy(A);
    }

    // 示例9: 转置操作 (SPARSE_OPERATION_TRANSPOSE)
    cout << "\n=== 9. 转置操作 (A^T * x) ===" << endl;
    {
        const int m = 3, n = 4;

        vector<int> row_ptr = {0, 2, 4, 6};
        vector<int> col_idx = {0, 2, 1, 3, 0, 2};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        sparse_matrix_t A;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                               row_ptr.data(), row_ptr.data() + 1,
                               col_idx.data(), values.data());

        // 计算 A^T * x (其中 x 是 m 维向量，结果 y 是 n 维向量)
        vector<double> x(m, 1.0);  // m 维输入向量
        vector<double> y(n, 0.0);  // n 维输出向量

        sparse_status_t status = mkl_sparse_d_mv(SPARSE_OPERATION_TRANSPOSE, 1.0, A, descr,
                                                 x.data(), 0.0, y.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "原矩阵A (" << m << "x" << n << "):" << endl;
            print_sparse_csr("A", m, n, row_ptr.data(), col_idx.data(), values.data(), 10);

            cout << "输入向量 x (维度 " << m << "):" << endl;
            for (int i = 0; i < m; ++i) {
                cout << x[i] << " ";
            }
            cout << endl;

            cout << "转置结果 y = A^T * x (维度 " << n << "):" << endl;
            for (int i = 0; i < n; ++i) {
                cout << y[i] << " ";
            }
            cout << endl << endl;
        }

        mkl_sparse_destroy(A);
    }

    // 示例10: 稀疏矩阵×稀疏矩阵乘法 (SpSpMM)
    cout << "\n=== 10. 稀疏矩阵×稀疏矩阵乘法 (SpSpMM) ===" << endl;
    {
        const int m = 3, n = 3, k = 3;

        // 矩阵A (3x3)
        vector<int> row_ptr_A = {0, 2, 4, 6};
        vector<int> col_idx_A = {0, 1, 1, 2, 0, 2};
        vector<double> values_A = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        // 矩阵B (3x3)
        vector<int> row_ptr_B = {0, 2, 3, 5};
        vector<int> col_idx_B = {0, 2, 1, 0, 2};
        vector<double> values_B = {1.0, 2.0, 3.0, 4.0, 5.0};

        sparse_matrix_t A, B, C;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                row_ptr_A.data(), row_ptr_A.data() + 1,
                                col_idx_A.data(), values_A.data());

        mkl_sparse_d_create_csr(&B, SPARSE_INDEX_BASE_ZERO, n, k,
                                row_ptr_B.data(), row_ptr_B.data() + 1,
                                col_idx_B.data(), values_B.data());

        // C = A * B (稀疏×稀疏)
        sparse_status_t status = mkl_sparse_spmm(SPARSE_OPERATION_NON_TRANSPOSE, A, B, &C);

        if (status == SPARSE_STATUS_SUCCESS) {
            sparse_index_base_t indexing;
            int rows, cols;
            int* rows_start, *rows_end, *col_idx_C;
            double* values_C;

            mkl_sparse_d_export_csr(C, &indexing, &rows, &cols, &rows_start, &rows_end, &col_idx_C, &values_C);

            cout << "矩阵A:" << endl;
            print_sparse_csr("A", m, n, row_ptr_A.data(), col_idx_A.data(), values_A.data(), 10);

            cout << "矩阵B:" << endl;
            print_sparse_csr("B", n, k, row_ptr_B.data(), col_idx_B.data(), values_B.data(), 10);

            cout << "结果矩阵C = A * B:" << endl;
            print_sparse_csr("C", rows, cols, rows_start, col_idx_C, values_C, 10);
        }

        mkl_sparse_destroy(A);
        mkl_sparse_destroy(B);
        mkl_sparse_destroy(C);
    }

    // 示例11: 矩阵导出和格式转换
    cout << "\n=== 11. 矩阵导出和格式转换 ===" << endl;
    {
        const int m = 3, n = 4;

        vector<int> row_ptr = {0, 2, 4, 6};
        vector<int> col_idx = {0, 2, 1, 3, 0, 2};
        vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        sparse_matrix_t A;
        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                               row_ptr.data(), row_ptr.data() + 1,
                               col_idx.data(), values.data());

        // 导出CSR格式
        sparse_index_base_t indexing;
        int rows, cols;
        int* rows_start, *rows_end, *col_idx_export;
        double* values_export;

        sparse_status_t status = mkl_sparse_d_export_csr(A, &indexing, &rows, &cols,
                                                          &rows_start, &rows_end,
                                                          &col_idx_export, &values_export);

        if (status == SPARSE_STATUS_SUCCESS) {
            cout << "导出CSR格式矩阵:" << endl;
            cout << "索引基址: " << (indexing == SPARSE_INDEX_BASE_ZERO ? "ZERO" : "ONE") << endl;
            cout << "矩阵大小: " << rows << "x" << cols << endl;
            print_sparse_csr("导出矩阵", rows, cols, rows_start, col_idx_export, values_export, 10);
        }

        // 转换为转置矩阵
        sparse_matrix_t AT;
        status = mkl_sparse_convert_csr(A, SPARSE_OPERATION_TRANSPOSE, &AT);
        if (status == SPARSE_STATUS_SUCCESS) {
            int* rows_start_T, *rows_end_T, *col_idx_T;
            double* values_T;
            mkl_sparse_d_export_csr(AT, &indexing, &rows, &cols,
                                    &rows_start_T, &rows_end_T,
                                    &col_idx_T, &values_T);
            cout << "转置后导出:" << endl;
            print_sparse_csr("A^T", rows, cols, rows_start_T, col_idx_T, values_T, 10);
            mkl_sparse_destroy(AT);
        }

        mkl_sparse_destroy(A);
    }

    // 示例12: 性能优化 - 多次运算对比
    cout << "\n=== 12. 性能优化 - 多次运算对比 ===" << endl;
    {
        const int m = 100, n = 100;
        const int num_iterations = 1000;

        // 创建较大的稀疏矩阵（三对角矩阵）
        vector<int> row_ptr(m + 1);
        vector<int> col_idx;
        vector<double> values;

        row_ptr[0] = 0;
        for (int i = 0; i < m; ++i) {
            int count = 0;
            if (i > 0) {
                col_idx.push_back(i - 1);
                values.push_back(-1.0);
                count++;
            }
            col_idx.push_back(i);
            values.push_back(2.0);
            count++;
            if (i < m - 1) {
                col_idx.push_back(i + 1);
                values.push_back(-1.0);
                count++;
            }
            row_ptr[i + 1] = row_ptr[i] + count;
        }

        sparse_matrix_t A;
        vector<int> row_start(m);
        vector<int> row_end(m);
        for (int i = 0; i < m; ++i) {
            row_start[i] = row_ptr[i];
            row_end[i] = row_ptr[i + 1];
        }

        mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, m, n,
                                row_start.data(), row_end.data(),
                                col_idx.data(), values.data());

        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;

        vector<double> x(n, 1.0);
        vector<double> y(m, 0.0);

        // 预热
        for (int i = 0; i < 10; ++i) {
            mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, descr,
                           x.data(), 0.0, y.data());
        }

        // 性能测试
        auto start = high_resolution_clock::now();
        for (int i = 0; i < num_iterations; ++i) {
            mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, descr,
                           x.data(), 0.0, y.data());
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "矩阵大小: " << m << "x" << n << endl;
        cout << "非零元素: " << row_ptr[m] << endl;
        cout << "迭代次数: " << num_iterations << endl;
        cout << "总耗时: " << duration.count() << " 微秒" << endl;
        cout << "平均每次运算: " << (double)duration.count() / num_iterations << " 微秒" << endl;
        cout << "吞吐量: " << (double)num_iterations * 1e6 / duration.count() << " 次/秒" << endl << endl;

        // 显示MKL线程数信息
        int num_threads = mkl_get_max_threads();
        cout << "MKL最大线程数: " << num_threads << endl;
        cout << "建议: 可以通过 mkl_set_num_threads() 调整线程数以优化性能" << endl << endl;

        mkl_sparse_destroy(A);
    }

    // 总结
    cout << "\n=== 总结 ===" << endl;
    cout << "MKL 稀疏BLAS模块功能:" << endl;
    cout << "1. 支持多种存储格式: CSR, CSC, COO" << endl;
    cout << "2. 矩阵创建和销毁: create_csr/csc/coo, destroy" << endl;
    cout << "3. 矩阵导出: export_csr/csc" << endl;
    cout << "4. 格式转换: convert_csr" << endl;
    cout << "5. 矩阵描述符: 支持GENERAL, SYMMETRIC等类型" << endl;
    cout << "6. 基本运算: SpMV, SpMM, SpSpMM, 矩阵加法" << endl;
    cout << "7. 转置操作: 支持转置和共轭转置" << endl;
    cout << "8. 性能优化: 多线程并行, SIMD向量化" << endl;
    cout << "9. 运算类型: NON_TRANSPOSE, TRANSPOSE, CONJUGATE_TRANSPOSE" << endl << endl;

    cin.get();
    return 0;
}
