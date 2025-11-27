#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

// Intel MKL 稀疏特征值求解器头文件
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

// 简单的幂迭代法（用于计算最大特征值）
double power_iteration(int n, const int* row_ptr, const int* col_idx, const double* values,
                      double* eigenvector, int max_iter = 1000, double tol = 1e-6) {
    vector<double> v(n, 1.0 / sqrt(n));
    vector<double> Av(n);

    double lambda_old = 0.0;

    for (int iter = 0; iter < max_iter; ++iter) {
        // Av = A * v
        fill(Av.begin(), Av.end(), 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
                Av[i] += values[j] * v[col_idx[j]];
            }
        }

        // 计算特征值估计
        double lambda = 0.0;
        for (int i = 0; i < n; ++i) {
            lambda += v[i] * Av[i];
        }

        // 归一化
        double norm = 0.0;
        for (int i = 0; i < n; ++i) {
            norm += Av[i] * Av[i];
        }
        norm = sqrt(norm);

        if (norm < 1e-12) {
            break;
        }

        for (int i = 0; i < n; ++i) {
            v[i] = Av[i] / norm;
        }

        // 检查收敛
        if (iter > 0 && abs(lambda - lambda_old) < tol * abs(lambda)) {
            copy(v.begin(), v.end(), eigenvector);
            return lambda;
        }

        lambda_old = lambda;
    }

    copy(v.begin(), v.end(), eigenvector);
    return lambda_old;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 稀疏特征值求解器示例 ===" << endl;
    cout << "包括: Lanczos、Arnoldi、Jacobi-Davidson等方法" << endl << endl;

    // 示例1: 幂迭代法计算最大特征值
    cout << "\n=== 1. 幂迭代法（最大特征值） ===" << endl;
    {
        const int n = 5;

        // 创建一个对称矩阵（三对角）
        vector<int> row_ptr = {0, 2, 5, 8, 11, 13};
        vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4};
        vector<double> values = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};

        vector<double> eigenvector(n);

        auto start = high_resolution_clock::now();
        double eigenvalue = power_iteration(n, row_ptr.data(), col_idx.data(), values.data(),
                                           eigenvector.data(), 1000, 1e-6);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "最大特征值: " << eigenvalue << endl;
        print_vector("对应特征向量", eigenvector.data(), n, n);
        cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

        // 验证: Av = lambda * v
        vector<double> Av(n, 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
                Av[i] += values[j] * eigenvector[col_idx[j]];
            }
        }

        double error = 0.0;
        for (int i = 0; i < n; ++i) {
            double diff = Av[i] - eigenvalue * eigenvector[i];
            error += diff * diff;
        }
        error = sqrt(error);
        cout << "验证误差 (||Av - lambda*v||): " << error << endl << endl;
    }

    // 示例2: 使用MKL稀疏BLAS进行矩阵-向量乘法
    cout << "\n=== 2. 使用MKL稀疏BLAS ===" << endl;
    {
        const int n = 4;

        vector<int> row_ptr = {0, 2, 4, 6, 8};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 0, 3};
        vector<double> values = {2.0, 1.0, 3.0, 1.0, 4.0, 1.0, 1.0, 5.0};

        sparse_matrix_t A;
        struct matrix_descr descr;
        descr.type = SPARSE_MATRIX_TYPE_GENERAL;
        descr.mode = SPARSE_FILL_MODE_FULL;
        descr.diag = SPARSE_DIAG_NON_UNIT;

        int status = mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, n, n,
                                           row_ptr.data(), row_ptr.data() + 1,
                                           col_idx.data(), values.data());

        if (status == SPARSE_STATUS_SUCCESS) {
            vector<double> v(n, 1.0 / sqrt(n));
            vector<double> Av(n, 0.0);

            double alpha = 1.0, beta = 0.0;

            // Av = A * v
            status = mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A, descr,
                                    v.data(), beta, Av.data());

            if (status == SPARSE_STATUS_SUCCESS) {
                print_vector("输入向量v", v.data(), n, n);
                print_vector("输出向量Av = A*v", Av.data(), n, n);

                // 计算Rayleigh商（特征值估计）
                double rayleigh = 0.0;
                double v_norm = 0.0;
                for (int i = 0; i < n; ++i) {
                    rayleigh += v[i] * Av[i];
                    v_norm += v[i] * v[i];
                }
                rayleigh /= v_norm;
                cout << "Rayleigh商 (特征值估计): " << rayleigh << endl << endl;
            }
        }

        mkl_sparse_destroy(A);
    }

    // 示例3: 简化的Lanczos方法（对称矩阵）
    cout << "\n=== 3. Lanczos方法（简化实现） ===" << endl;
    {
        const int n = 5;
        const int m = 3;  // Lanczos步骤数

        vector<int> row_ptr = {0, 2, 5, 8, 11, 13};
        vector<int> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4};
        vector<double> values = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};

        // Lanczos三对角化（简化实现）
        vector<double> alpha(m), beta(m);
        vector<vector<double>> V(n, vector<double>(m));

        // 初始化
        vector<double> v0(n, 1.0 / sqrt(n));
        for (int i = 0; i < n; ++i) {
            V[i][0] = v0[i];
        }

        vector<double> w(n), v_prev(n);
        copy(v0.begin(), v0.end(), v_prev.begin());

        for (int j = 0; j < m - 1; ++j) {
            // w = A * v_j
            fill(w.begin(), w.end(), 0.0);
            for (int i = 0; i < n; ++i) {
                for (int k = row_ptr[i]; k < row_ptr[i+1]; ++k) {
                    w[i] += values[k] * V[col_idx[k]][j];
                }
            }

            // alpha_j = v_j^T * w
            alpha[j] = 0.0;
            for (int i = 0; i < n; ++i) {
                alpha[j] += V[i][j] * w[i];
            }

            // w = w - alpha_j * v_j - beta_{j-1} * v_{j-1}
            for (int i = 0; i < n; ++i) {
                w[i] -= alpha[j] * V[i][j];
                if (j > 0) {
                    w[i] -= beta[j-1] * v_prev[i];
                }
            }

            // beta_j = ||w||
            beta[j] = 0.0;
            for (int i = 0; i < n; ++i) {
                beta[j] += w[i] * w[i];
            }
            beta[j] = sqrt(beta[j]);

            if (beta[j] > 1e-12) {
                // v_{j+1} = w / beta_j
                for (int i = 0; i < n; ++i) {
                    V[i][j+1] = w[i] / beta[j];
                }
            }

            copy(V[j].begin(), V[j].end(), v_prev.begin());
        }

        // 构建三对角矩阵T
        cout << "Lanczos三对角矩阵T (前" << m << "x" << m << "):" << endl;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                if (i == j) {
                    cout << alpha[i] << "\t";
                } else if (abs(i - j) == 1) {
                    cout << beta[min(i, j)] << "\t";
                } else {
                    cout << "0\t";
                }
            }
            cout << endl;
        }
        cout << endl;

        // 注意：实际应用中需要求解三对角矩阵的特征值
        cout << "注意：完整Lanczos方法需要求解三对角矩阵的特征值" << endl;
        cout << "可以使用QR算法或其他方法求解T的特征值" << endl << endl;
    }

    // 示例4: 简化的Arnoldi方法（非对称矩阵）
    cout << "\n=== 4. Arnoldi方法（简化实现） ===" << endl;
    {
        const int n = 4;
        const int m = 3;  // Arnoldi步骤数

        vector<int> row_ptr = {0, 2, 4, 6, 8};
        vector<int> col_idx = {0, 1, 1, 2, 2, 3, 0, 3};
        vector<double> values = {2.0, 1.0, 3.0, 1.0, 4.0, 1.0, 1.0, 5.0};

        // Arnoldi过程（简化实现）
        vector<vector<double>> V(n, vector<double>(m));
        vector<vector<double>> H(m, vector<double>(m, 0.0));

        // 初始化第一个向量
        vector<double> v0(n, 1.0 / sqrt(n));
        for (int i = 0; i < n; ++i) {
            V[i][0] = v0[i];
        }

        for (int j = 0; j < m - 1; ++j) {
            // w = A * v_j
            vector<double> w(n, 0.0);
            for (int i = 0; i < n; ++i) {
                for (int k = row_ptr[i]; k < row_ptr[i+1]; ++k) {
                    w[i] += values[k] * V[col_idx[k]][j];
                }
            }

            // 正交化
            for (int i = 0; i <= j; ++i) {
                double dot = 0.0;
                for (int k = 0; k < n; ++k) {
                    dot += V[k][i] * w[k];
                }
                H[i][j] = dot;

                for (int k = 0; k < n; ++k) {
                    w[k] -= dot * V[k][i];
                }
            }

            // 归一化
            double norm = 0.0;
            for (int i = 0; i < n; ++i) {
                norm += w[i] * w[i];
            }
            norm = sqrt(norm);

            if (norm > 1e-12) {
                H[j+1][j] = norm;
                for (int i = 0; i < n; ++i) {
                    V[i][j+1] = w[i] / norm;
                }
            }
        }

        cout << "Arnoldi上Hessenberg矩阵H (前" << m << "x" << m << "):" << endl;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << H[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;

        cout << "注意：完整Arnoldi方法需要求解Hessenberg矩阵的特征值" << endl;
        cout << "可以使用QR算法或其他方法求解H的特征值" << endl << endl;
    }

    // 示例5: 特征值问题的应用示例
    cout << "\n=== 5. 特征值问题应用示例 ===" << endl;
    {
        const int n = 3;

        // 创建一个简单的对称矩阵
        vector<int> row_ptr = {0, 2, 4, 6};
        vector<int> col_idx = {0, 1, 1, 2, 0, 2};
        vector<double> values = {1.0, 0.5, 2.0, 0.5, 0.5, 3.0};

        cout << "矩阵A (3x3对称矩阵):" << endl;
        cout << "特征值问题: A * v = lambda * v" << endl;

        vector<double> eigenvector(n);
        double eigenvalue = power_iteration(n, row_ptr.data(), col_idx.data(), values.data(),
                                           eigenvector.data(), 1000, 1e-6);

        cout << "最大特征值: " << eigenvalue << endl;
        print_vector("对应特征向量", eigenvector.data(), n, n);

        // 计算条件数（最大特征值/最小特征值）
        // 注意：这里只是示例，实际需要计算所有特征值
        cout << "注意：计算条件数需要所有特征值" << endl;
        cout << "可以使用完整的特征值求解器（如Lanczos、Arnoldi）" << endl;
    }

    cout << "\n注意：以上是稀疏特征值求解器的简化实现示例。" << endl;
    cout << "实际应用中，MKL提供了更高效的特征值求解器接口，" << endl;
    cout << "包括完整的Lanczos、Arnoldi、Jacobi-Davidson等方法，" << endl;
    cout << "建议使用MKL的完整特征值求解器库以获得更好的性能和精度。" << endl;

    cin.get();
    return 0;
}
