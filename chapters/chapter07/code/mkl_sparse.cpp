#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

// Intel MKL 稀疏矩阵求解头文件
#include <mkl_spblas.h>
#include <mkl_sparse_qr.h>
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_vector(const char* name, const double* vector, int size) {
    cout << name << ":" << endl;
    for (int i = 0; i < size; ++i) {
        cout << vector[i] << " ";
    }
    cout << endl << endl;
}

// 计算向量内积
double dot_product(const double* a, const double* b, int n) {
    return cblas_ddot(n, a, 1, b, 1);
}

// 计算向量2-范数
double vector_norm(const double* a, int n) {
    return cblas_dnrm2(n, a, 1);
}

// 向量缩放: y = alpha * x
void vector_scale(double alpha, const double* x, double* y, int n) {
    cblas_dcopy(n, x, 1, y, 1);
    cblas_dscal(n, alpha, y, 1);
}

// 向量线性组合: y = alpha * x + beta * y
void vector_axpy(double alpha, const double* x, double beta, double* y, int n) {
    if (beta != 1.0) {
        cblas_dscal(n, beta, y, 1);
    }
    cblas_daxpy(n, alpha, x, 1, y, 1);
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 稀疏矩阵求解示例 ===" << endl;

    // 创建一个稀疏矩阵 (CSR格式)
    // 示例矩阵: 3x3
    // [ 2  -1   0 ]
    // [-1   2  -1 ]
    // [ 0  -1   2 ]

    const int n = 3;  // 矩阵大小
    const int nnz = 7;  // 非零元素个数

    // CSR格式: 行索引数组
    vector<int> row_index = {0, 2, 5, 7};

    // CSR格式: 列索引数组
    vector<int> col_index = {0, 1, 0, 1, 2, 1, 2};

    // CSR格式: 非零元素值
    vector<double> values = {2.0, -1.0, -1.0, 2.0, -1.0, -1.0, 2.0};

    // 右端向量 b
    vector<double> b = {1.0, 0.0, 1.0};

    // 解向量 x
    vector<double> x(n, 0.0);

    cout << "稀疏矩阵 (CSR格式):" << endl;
    cout << "行索引: ";
    for (int i = 0; i < row_index.size(); ++i) {
        cout << row_index[i] << " ";
    }
    cout << endl;
    cout << "列索引: ";
    for (int i = 0; i < col_index.size(); ++i) {
        cout << col_index[i] << " ";
    }
    cout << endl;
    cout << "非零值: ";
    for (int i = 0; i < values.size(); ++i) {
        cout << values[i] << " ";
    }
    cout << endl << endl;

    print_vector("右端向量b", b.data(), n);

    // 创建稀疏矩阵句柄
    sparse_matrix_t A = nullptr;
    struct matrix_descr descrA;
    descrA.type = SPARSE_MATRIX_TYPE_GENERAL;
    descrA.mode = SPARSE_FILL_MODE_FULL;
    descrA.diag = SPARSE_DIAG_NON_UNIT;

    // 创建CSR格式的稀疏矩阵
    mkl_sparse_d_create_csr(&A,
                            SPARSE_INDEX_BASE_ZERO,
                            n, n,
                            row_index.data(),
                            row_index.data() + 1,
                            col_index.data(),
                            values.data());

    if (A == nullptr) {
        cout << "创建稀疏矩阵失败" << endl;
        return 1;
    }

    // ============================================
    // 方法1: 使用迭代求解器 (共轭梯度法 CG)
    // ============================================
    cout << "\n=== 方法1: 共轭梯度法 (Conjugate Gradient) ===" << endl;

    // 共轭梯度法参数
    const int max_iter = 1000;  // 最大迭代次数
    const double tolerance = 1e-10;  // 收敛容差
    vector<double> x_cg(n, 0.0);  // 初始解向量
    vector<double> r(n);  // 残差向量 r = b - A*x
    vector<double> p(n);  // 搜索方向
    vector<double> Ap(n);  // A*p

    // 初始化残差: r = b - A*x0
    cblas_dcopy(n, b.data(), 1, r.data(), 1);
    mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE,
                    1.0, A, descrA,
                    x_cg.data(),
                    -1.0, r.data());

    // 初始化搜索方向: p = r
    cblas_dcopy(n, r.data(), 1, p.data(), 1);

    double r_norm_sq = dot_product(r.data(), r.data(), n);
    double r0_norm_sq = r_norm_sq;
    int iter = 0;

    auto start_cg = high_resolution_clock::now();

    // 共轭梯度法迭代
    while (iter < max_iter && r_norm_sq > tolerance * tolerance * r0_norm_sq) {
        // 计算 Ap = A * p
        mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE,
                        1.0, A, descrA,
                        p.data(),
                        0.0, Ap.data());

        // 计算 alpha = (r^T * r) / (p^T * A * p)
        double pAp = dot_product(p.data(), Ap.data(), n);
        if (abs(pAp) < 1e-15) {
            cout << "警告: 矩阵可能不是正定的，pAp = " << pAp << endl;
            break;
        }
        double alpha = r_norm_sq / pAp;

        // 更新解: x = x + alpha * p
        vector_axpy(alpha, p.data(), 1.0, x_cg.data(), n);

        // 更新残差: r = r - alpha * A * p
        vector_axpy(-alpha, Ap.data(), 1.0, r.data(), n);

        // 计算新的残差范数平方
        double r_norm_sq_new = dot_product(r.data(), r.data(), n);

        // 计算 beta = (r_new^T * r_new) / (r^T * r)
        double beta = r_norm_sq_new / r_norm_sq;

        // 更新搜索方向: p = r + beta * p
        vector_axpy(1.0, r.data(), beta, p.data(), n);

        r_norm_sq = r_norm_sq_new;
        iter++;
    }

    auto end_cg = high_resolution_clock::now();
    auto duration_cg = duration_cast<microseconds>(end_cg - start_cg);

    double final_residual = sqrt(r_norm_sq);
    print_vector("解向量x (共轭梯度法)", x_cg.data(), n);
    cout << "迭代次数: " << iter << endl;
    cout << "最终残差: " << final_residual << endl;
    cout << "求解耗时: " << duration_cg.count() << " 微秒" << endl;

    // 验证解
    vector<double> residual_cg(n, 0.0);
    cblas_dcopy(n, b.data(), 1, residual_cg.data(), 1);
    mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE,
                    1.0, A, descrA,
                    x_cg.data(),
                    -1.0, residual_cg.data());
    double max_residual_cg = 0.0;
    for (int i = 0; i < n; ++i) {
        max_residual_cg = max(max_residual_cg, abs(residual_cg[i]));
    }
    cout << "验证残差 ||Ax - b||_inf: " << max_residual_cg << endl << endl;

    // ============================================
    // 方法2: 使用稀疏QR分解求解器
    // ============================================
    cout << "=== 方法2: 稀疏QR分解求解器 ===" << endl;

    sparse_matrix_t A_qr = nullptr;
    struct matrix_descr descrA_qr;
    descrA_qr.type = SPARSE_MATRIX_TYPE_GENERAL;
    descrA_qr.mode = SPARSE_FILL_MODE_FULL;
    descrA_qr.diag = SPARSE_DIAG_NON_UNIT;

    // 重新创建矩阵（QR分解需要）
    mkl_sparse_d_create_csr(&A_qr,
                            SPARSE_INDEX_BASE_ZERO,
                            n, n,
                            row_index.data(),
                            row_index.data() + 1,
                            col_index.data(),
                            values.data());

    if (A_qr == nullptr) {
        cout << "创建稀疏矩阵失败（QR）" << endl;
        mkl_sparse_destroy(A);
        return 1;
    }

    // 创建QR分解句柄
    // sparse_status_t status;  // 未使用，注释掉
    sparse_matrix_t Q = nullptr, R = nullptr;

    // 注意: MKL的稀疏QR分解API较新，这里展示基本用法
    // 实际使用中可能需要根据MKL版本调整

    cout << "注意: 稀疏QR分解需要MKL 2020.0或更高版本" << endl;
    cout << "这里展示QR分解的基本思路" << endl;

    // 对于小规模矩阵，可以使用稠密QR分解作为对比
    cout << "\n对比: 使用稠密QR分解" << endl;

    // 将稀疏矩阵转换为稠密矩阵进行QR分解
    vector<vector<double>> dense_A_qr(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = row_index[i]; j < row_index[i+1]; ++j) {
            dense_A_qr[i][col_index[j]] = values[j];
        }
    }

    // 使用LAPACK的稠密QR分解 (dgeqrf)
    vector<double> tau(n);  // 反射向量
    vector<double> work(n * n);
    int lwork = n * n;
    // int info;  // 未使用，注释掉

    // 复制矩阵到工作数组
    vector<double> A_qr_work(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A_qr_work[i + j * n] = dense_A_qr[i][j];
        }
    }

    auto start_qr = high_resolution_clock::now();

    // QR分解: A = Q * R
    LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, n, n, A_qr_work.data(), n, tau.data());

    // 提取R矩阵（上三角部分）
    vector<vector<double>> R_dense(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            R_dense[i][j] = A_qr_work[i + j * n];
        }
    }

    // 构建Q矩阵
    vector<double> Q_work(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Q_work[i + j * n] = (i == j) ? 1.0 : 0.0;
        }
    }
    LAPACKE_dorgqr(LAPACK_ROW_MAJOR, n, n, n, Q_work.data(), n, tau.data());

    // 求解: R * x = Q^T * b
    vector<double> b_qr(n);
    // 计算 Q^T * b
    cblas_dgemv(CblasRowMajor, CblasTrans, n, n, 1.0, Q_work.data(), n,
                b.data(), 1, 0.0, b_qr.data(), 1);

    // 回代求解 R * x = Q^T * b
    vector<double> x_qr(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        x_qr[i] = b_qr[i];
        for (int j = i + 1; j < n; ++j) {
            x_qr[i] -= R_dense[i][j] * x_qr[j];
        }
        x_qr[i] /= R_dense[i][i];
    }

    auto end_qr = high_resolution_clock::now();
    auto duration_qr = duration_cast<microseconds>(end_qr - start_qr);

    print_vector("解向量x (QR分解)", x_qr.data(), n);
    cout << "QR分解求解耗时: " << duration_qr.count() << " 微秒" << endl;

    // 验证解
    vector<double> residual_qr(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            residual_qr[i] += dense_A_qr[i][j] * x_qr[j];
        }
        residual_qr[i] -= b[i];
    }
    double max_residual_qr = 0.0;
    for (int i = 0; i < n; ++i) {
        max_residual_qr = max(max_residual_qr, abs(residual_qr[i]));
    }
    cout << "验证残差 ||Ax - b||_inf: " << max_residual_qr << endl << endl;

    // 释放QR相关资源
    if (A_qr != nullptr) {
        mkl_sparse_destroy(A_qr);
    }

    // 释放原始矩阵句柄
    mkl_sparse_destroy(A);

    // 示例: 使用稠密矩阵求解作为对比
    cout << "\n=== 对比: 稠密矩阵求解 ===" << endl;

    // 将稀疏矩阵转换为稠密矩阵
    vector<vector<double>> dense_A(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = row_index[i]; j < row_index[i+1]; ++j) {
            dense_A[i][col_index[j]] = values[j];
        }
    }

    cout << "稠密矩阵A:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << dense_A[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // 使用高斯消元法求解 (简化示例)
    vector<double> x_dense = b;

    // 前向消元
    for (int k = 0; k < n - 1; ++k) {
        for (int i = k + 1; i < n; ++i) {
            double factor = dense_A[i][k] / dense_A[k][k];
            for (int j = k; j < n; ++j) {
                dense_A[i][j] -= factor * dense_A[k][j];
            }
            x_dense[i] -= factor * x_dense[k];
        }
    }

    // 回代
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            x_dense[i] -= dense_A[i][j] * x_dense[j];
        }
        x_dense[i] /= dense_A[i][i];
    }

    print_vector("解向量x (稠密求解)", x_dense.data(), n);

    // 验证解
    vector<double> residual(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            residual[i] += dense_A[i][j] * x_dense[j];
        }
        residual[i] -= b[i];
    }
    print_vector("残差 A*x - b", residual.data(), n);

    // ============================================
    // 性能对比与总结
    // ============================================
    cout << "\n=== 性能对比与总结 ===" << endl;
    cout << "\n存储效率:" << endl;
    cout << "  稀疏矩阵存储: " << nnz << " 个非零元素" << endl;
    cout << "  稠密矩阵存储: " << n * n << " 个元素" << endl;
    cout << "  存储节省: " << (1.0 - (double)nnz / (n * n)) * 100 << "%" << endl;

    cout << "\n求解方法对比:" << endl;
    cout << "  1. 共轭梯度法 (CG):" << endl;
    cout << "     - 迭代次数: " << iter << endl;
    cout << "     - 求解时间: " << duration_cg.count() << " 微秒" << endl;
    cout << "     - 残差: " << max_residual_cg << endl;
    cout << "     - 适用: 对称正定矩阵" << endl;
    cout << "     - 优点: 内存需求小，适合大规模问题" << endl;

    cout << "\n  2. QR分解:" << endl;
    cout << "     - 求解时间: " << duration_qr.count() << " 微秒" << endl;
    cout << "     - 残差: " << max_residual_qr << endl;
    cout << "     - 适用: 一般矩阵（包括超定系统）" << endl;
    cout << "     - 优点: 数值稳定，适合病态矩阵" << endl;

    cout << "\n  3. 稠密矩阵求解 (高斯消元):" << endl;
    cout << "     - 适用: 小规模稠密矩阵" << endl;
    cout << "     - 缺点: 内存和计算量大" << endl;

    cout << "\n选择建议:" << endl;
    cout << "  - 对称正定矩阵 → 共轭梯度法 (CG)" << endl;
    cout << "  - 一般矩阵 → QR分解或Pardiso" << endl;
    cout << "  - 多次求解同一矩阵 → 直接法（重用分解）" << endl;
    cout << "  - 大规模稀疏矩阵 → 稀疏求解器" << endl;
    cout << "  - 对于大规模稀疏矩阵，稀疏求解器可以显著提高性能" << endl;

    cout << "\n=== 示例完成 ===" << endl;

    cin.get();
    return 0;
}
