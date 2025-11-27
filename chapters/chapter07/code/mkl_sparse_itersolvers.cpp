#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

// Intel MKL 稀疏迭代求解器头文件
#include <mkl.h>
#include <mkl_spblas.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_vector(const char* name, const double* vec, int size, int max_print = 10) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << vec[i] << " ";
    }
    if (size > max_print) {
        cout << "... (共 " << size << " 个元素)";
    }
    cout << endl << endl;
}

// 简单的矩阵-向量乘法（用于迭代求解器）
void sparse_matvec(int n, const int* row_ptr, const int* col_idx, const double* values,
                   const double* x, double* y) {
    fill(y, y + n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
            y[i] += values[j] * x[col_idx[j]];
        }
    }
}

// 共轭梯度法 (CG) - 对称正定矩阵
void conjugate_gradient(int n, const int* row_ptr, const int* col_idx, const double* values,
                        const double* b, double* x, double tol = 1e-6, int max_iter = 1000) {
    vector<double> r(n), p(n), Ap(n);

    // 初始化
    copy(b, b + n, r.begin());
    sparse_matvec(n, row_ptr, col_idx, values, x, Ap.data());
    for (int i = 0; i < n; ++i) {
        r[i] -= Ap[i];
    }
    copy(r.begin(), r.end(), p.begin());

    double rnorm0 = 0.0;
    for (int i = 0; i < n; ++i) {
        rnorm0 += r[i] * r[i];
    }
    rnorm0 = sqrt(rnorm0);

    if (rnorm0 < tol) {
        cout << "初始残差已满足收敛条件" << endl;
        return;
    }

    for (int iter = 0; iter < max_iter; ++iter) {
        sparse_matvec(n, row_ptr, col_idx, values, p.data(), Ap.data());

        double pAp = 0.0;
        for (int i = 0; i < n; ++i) {
            pAp += p[i] * Ap[i];
        }

        double alpha = 0.0;
        for (int i = 0; i < n; ++i) {
            alpha += r[i] * r[i];
        }
        alpha /= pAp;

        for (int i = 0; i < n; ++i) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        double rnorm = 0.0;
        for (int i = 0; i < n; ++i) {
            rnorm += r[i] * r[i];
        }
        rnorm = sqrt(rnorm);

        if (rnorm / rnorm0 < tol) {
            cout << "CG收敛于迭代 " << iter + 1 << ", 相对残差: " << rnorm / rnorm0 << endl;
            return;
        }

        double beta = 0.0;
        for (int i = 0; i < n; ++i) {
            beta += r[i] * r[i];
        }
        double beta_prev = 0.0;
        for (int i = 0; i < n; ++i) {
            beta_prev += (r[i] + alpha * Ap[i]) * (r[i] + alpha * Ap[i]);
        }
        beta /= beta_prev;

        for (int i = 0; i < n; ++i) {
            p[i] = r[i] + beta * p[i];
        }
    }

    cout << "CG达到最大迭代次数" << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 稀疏迭代求解器示例 ===" << endl;
    cout << "包括: CG、BiCG、GMRES等方法" << endl << endl;

    // 示例1: 共轭梯度法 (CG) - 对称正定矩阵
    cout << "\n=== 1. 共轭梯度法 (CG) ===" << endl;
    {
        const int n = 5;

        // 创建一个对称正定矩阵（三对角）
        vector<int> row_ptr = {0, 2, 5, 8, 11, 13};
        vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4};
        vector<double> values = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};

        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        print_vector("右端向量b", b.data(), n, n);

        auto start = high_resolution_clock::now();
        conjugate_gradient(n, row_ptr.data(), col_idx.data(), values.data(),
                           b.data(), x.data(), 1e-6, 1000);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        print_vector("CG解向量x", x.data(), n, n);
        cout << "计算耗时: " << duration.count() << " 微秒" << endl;

        // 验证解
        vector<double> Ax(n);
        sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), x.data(), Ax.data());
        double error = 0.0;
        for (int i = 0; i < n; ++i) {
            error += (Ax[i] - b[i]) * (Ax[i] - b[i]);
        }
        error = sqrt(error);
        cout << "残差: " << error << endl << endl;
    }

    // 示例2: 使用MKL稀疏BLAS的矩阵-向量乘法
    cout << "\n=== 2. 使用MKL稀疏BLAS ===" << endl;
    {
        const int n = 4;

        vector<int> row_ptr = {0, 2, 4, 6, 8};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 0, 3};
        vector<double> values = {2.0, 1.0, 3.0, 1.0, 4.0, 1.0, 1.0, 5.0};

        vector<double> x(n, 1.0);
        vector<double> y(n, 0.0);

        sparse_matrix_t A;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;
        descr.mode = SPARSE_FILL_MODE_FULL;
        descr.diag = SPARSE_DIAG_NON_UNIT;

        int status = mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, n, n,
                                           row_ptr.data(), row_ptr.data() + 1,
                                           col_idx.data(), values.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            double alpha = 1.0, beta = 0.0;

            // y = A * x
            status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A, descr,
                                    x.data(), beta, y.data());

            if (status == SPARSE_STATUS_SUCCESS) {
                print_vector("输入向量x", x.data(), n, n);
                print_vector("输出向量y = A*x", y.data(), n, n);
            }
        }

        mkl_sparse_destroy(A);
    }

    // 示例3: 预条件共轭梯度法 (PCG) - 使用对角预条件
    cout << "\n=== 3. 预条件共轭梯度法 (PCG) ===" << endl;
    {
        const int n = 5;

        vector<int> row_ptr = {0, 2, 5, 8, 11, 13};
        vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4};
        vector<double> values = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};

        // 提取对角元素作为预条件
        vector<double> diag(n);
        for (int i = 0; i < n; ++i) {
            for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
                if (col_idx[j] == i) {
                    diag[i] = values[j];
                    break;
                }
            }
        }

        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        // 简化的PCG实现（使用对角预条件）
        vector<double> r(n), p(n), Ap(n), z(n);

        sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), x.data(), r.data());
        for (int i = 0; i < n; ++i) {
            r[i] = b[i] - r[i];
            z[i] = r[i] / diag[i];  // 预条件
        }
        copy(z.begin(), z.end(), p.begin());

        double rnorm0 = 0.0;
        for (int i = 0; i < n; ++i) {
            rnorm0 += r[i] * r[i];
        }
        rnorm0 = sqrt(rnorm0);

        for (int iter = 0; iter < 100; ++iter) {
            sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), p.data(), Ap.data());

            double pAp = 0.0;
            for (int i = 0; i < n; ++i) {
                pAp += p[i] * Ap[i];
            }

            double rz = 0.0;
            for (int i = 0; i < n; ++i) {
                rz += r[i] * z[i];
            }

            double alpha = rz / pAp;

            for (int i = 0; i < n; ++i) {
                x[i] += alpha * p[i];
                r[i] -= alpha * Ap[i];
            }

            double rnorm = 0.0;
            for (int i = 0; i < n; ++i) {
                rnorm += r[i] * r[i];
            }
            rnorm = sqrt(rnorm);

            if (rnorm / rnorm0 < 1e-6) {
                cout << "PCG收敛于迭代 " << iter + 1 << ", 相对残差: " << rnorm / rnorm0 << endl;
                break;
            }

            for (int i = 0; i < n; ++i) {
                z[i] = r[i] / diag[i];  // 预条件
            }

            double rz_new = 0.0;
            for (int i = 0; i < n; ++i) {
                rz_new += r[i] * z[i];
            }

            double beta = rz_new / rz;

            for (int i = 0; i < n; ++i) {
                p[i] = z[i] + beta * p[i];
            }
        }

        print_vector("PCG解向量x", x.data(), n, n);

        // 验证
        vector<double> Ax(n);
        sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), x.data(), Ax.data());
        double error = 0.0;
        for (int i = 0; i < n; ++i) {
            error += (Ax[i] - b[i]) * (Ax[i] - b[i]);
        }
        error = sqrt(error);
        cout << "残差: " << error << endl << endl;
    }

    // 示例4: BiCG方法（双共轭梯度法）- 非对称矩阵
    cout << "\n=== 4. BiCG方法（简化实现） ===" << endl;
    {
        const int n = 4;

        // 非对称矩阵
        vector<int> row_ptr = {0, 2, 4, 6, 8};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 0, 3};
        vector<double> values = {2.0, 1.0, 3.0, 1.0, 4.0, 1.0, 1.0, 5.0};

        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        // 简化的BiCG实现
        vector<double> r(n), r_tilde(n), p(n), p_tilde(n), Ap(n), A_T_p_tilde(n);

        sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), x.data(), r.data());
        for (int i = 0; i < n; ++i) {
            r[i] = b[i] - r[i];
            r_tilde[i] = r[i];
        }
        copy(r.begin(), r.end(), p.begin());
        copy(r_tilde.begin(), r_tilde.end(), p_tilde.begin());

        double rnorm0 = 0.0;
        for (int i = 0; i < n; ++i) {
            rnorm0 += r[i] * r[i];
        }
        rnorm0 = sqrt(rnorm0);

        for (int iter = 0; iter < 100; ++iter) {
            sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), p.data(), Ap.data());

            // 计算转置矩阵的乘法（简化：假设对称）
            sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), p_tilde.data(), A_T_p_tilde.data());

            double p_A_T_p_tilde = 0.0;
            for (int i = 0; i < n; ++i) {
                p_A_T_p_tilde += p[i] * A_T_p_tilde[i];
            }

            if (abs(p_A_T_p_tilde) < 1e-12) {
                cout << "BiCG失败：分母为零" << endl;
                break;
            }

            double r_r_tilde = 0.0;
            for (int i = 0; i < n; ++i) {
                r_r_tilde += r[i] * r_tilde[i];
            }

            double alpha = r_r_tilde / p_A_T_p_tilde;

            for (int i = 0; i < n; ++i) {
                x[i] += alpha * p[i];
                r[i] -= alpha * Ap[i];
                r_tilde[i] -= alpha * A_T_p_tilde[i];
            }

            double rnorm = 0.0;
            for (int i = 0; i < n; ++i) {
                rnorm += r[i] * r[i];
            }
            rnorm = sqrt(rnorm);

            if (rnorm / rnorm0 < 1e-6) {
                cout << "BiCG收敛于迭代 " << iter + 1 << ", 相对残差: " << rnorm / rnorm0 << endl;
                break;
            }

            double r_r_tilde_new = 0.0;
            for (int i = 0; i < n; ++i) {
                r_r_tilde_new += r[i] * r_tilde[i];
            }

            double beta = r_r_tilde_new / r_r_tilde;

            for (int i = 0; i < n; ++i) {
                p[i] = r[i] + beta * p[i];
                p_tilde[i] = r_tilde[i] + beta * p_tilde[i];
            }
        }

        print_vector("BiCG解向量x", x.data(), n, n);
    }

    // 示例5: GMRES方法（简化实现）
    cout << "\n=== 5. GMRES方法（简化实现） ===" << endl;
    {
        const int n = 4;
        const int m = 3;  // Krylov子空间维度

        vector<int> row_ptr = {0, 2, 4, 6, 8};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 0, 3};
        vector<double> values = {2.0, 1.0, 3.0, 1.0, 4.0, 1.0, 1.0, 5.0};

        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        // 简化的GMRES实现（Arnoldi过程）
        vector<double> r(n);
        sparse_matvec(n, row_ptr.data(), col_idx.data(), values.data(), x.data(), r.data());
        for (int i = 0; i < n; ++i) {
            r[i] = b[i] - r[i];
        }

        double beta = 0.0;
        for (int i = 0; i < n; ++i) {
            beta += r[i] * r[i];
        }
        beta = sqrt(beta);

        if (beta < 1e-6) {
            cout << "初始残差已满足收敛条件" << endl;
        } else {
            cout << "GMRES简化实现（基本Arnoldi过程）" << endl;
            cout << "初始残差: " << beta << endl;
            cout << "注意：完整GMRES需要QR分解和最小二乘求解" << endl;
        }

        print_vector("GMRES解向量x（初始）", x.data(), n, n);
    }

    cout << "\n注意：以上是迭代求解器的简化实现示例。" << endl;
    cout << "实际应用中，MKL提供了更高效的迭代求解器接口，" << endl;
    cout << "建议使用MKL的完整迭代求解器库以获得更好的性能。" << endl;

    cin.get();
    return 0;
}
