#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

// Intel MKL 稀疏直接求解器头文件
#include <mkl.h>
#include <mkl_spblas.h>
#include <mkl_pardiso.h>
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

void print_sparse_info(const char* name, int n, int nnz) {
    cout << name << ": " << n << "x" << n << " 矩阵, " << nnz << " 个非零元素" << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 稀疏直接求解器示例 ===" << endl;
    cout << "包括: Pardiso、稀疏LU、稀疏QR、稀疏Cholesky等" << endl << endl;

    // 示例1: Pardiso求解器 - 对称正定矩阵
    cout << "\n=== 1. Pardiso - 对称正定矩阵 ===" << endl;
    {
        const int n = 5;

        // 创建一个5x5对称正定矩阵（三对角矩阵）
        // [2  1  0  0  0]
        // [1  2  1  0  0]
        // [0  1  2  1  0]
        // [0  0  1  2  1]
        // [0  0  0  1  2]

        vector<int> ia = {1, 3, 6, 9, 12, 14};  // Fortran索引
        vector<int> ja = {1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5};
        vector<double> a = {2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0};
        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        print_sparse_info("矩阵A", n, ja.size());
        print_vector("右端向量b", b.data(), n);

        void* pt[64];
        int iparm[64];
        int maxfct = 1, mnum = 1;
        int mtype = 2;  // 对称正定矩阵
        int phase;
        int nrhs = 1, msglvl = 0, error = 0;

        // 初始化
        for (int i = 0; i < 64; ++i) {
            pt[i] = nullptr;
            iparm[i] = 0;
        }

        iparm[0] = 1;  // 不使用默认值
        iparm[1] = 2;  // 填充减少排序
        iparm[2] = 0;  // 线程数（0=自动）
        iparm[3] = 0;  // 预条件迭代求解器
        iparm[4] = 0;  // 用户排列
        iparm[5] = 0;  // 写解向量
        iparm[6] = 0;  // 输出格式
        iparm[7] = 2;  // 最大迭代细化步数
        iparm[8] = 0;  // 迭代细化
        iparm[9] = 13; // 扰动
        iparm[10] = 1; // 缩放向量
        iparm[11] = 0; // 共轭转置
        iparm[12] = 1; // 匹配
        iparm[13] = 0; // 输出
        iparm[14] = 0; // 峰值内存
        iparm[17] = -1; // 报告
        iparm[18] = -1; // 报告
        iparm[19] = 0; // 报告

        // 阶段1: 重排序和符号分解
        phase = 11;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        if (error != 0) {
            cout << "阶段1失败，错误代码: " << error << endl;
            return 1;
        }
        cout << "阶段1完成: 重排序和符号分解" << endl;

        // 阶段2: 数值分解
        phase = 22;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        if (error != 0) {
            cout << "阶段2失败，错误代码: " << error << endl;
            pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                    nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                    iparm, &msglvl, nullptr, nullptr, &error);
            return 1;
        }
        cout << "阶段2完成: 数值分解" << endl;

        // 阶段3: 前向和反向替换
        phase = 33;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, b.data(), x.data(), &error);

        if (error != 0) {
            cout << "阶段3失败，错误代码: " << error << endl;
        } else {
            print_vector("解向量x", x.data(), n);
        }

        // 阶段4: 释放内存
        phase = -1;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);
    }

    // 示例2: Pardiso - 非对称矩阵
    cout << "\n=== 2. Pardiso - 非对称矩阵 ===" << endl;
    {
        const int n = 4;

        // 创建一个4x4非对称矩阵
        vector<int> ia = {1, 3, 5, 7, 9};  // Fortran索引
        vector<int> ja = {1, 2, 2, 3, 3, 4, 1, 4, 4};
        vector<double> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        print_sparse_info("矩阵A (非对称)", n, ja.size());

        void* pt[64];
        int iparm[64];
        int maxfct = 1, mnum = 1;
        int mtype = 11;  // 非对称矩阵
        int phase;
        int nrhs = 1, msglvl = 0, error = 0;

        for (int i = 0; i < 64; ++i) {
            pt[i] = nullptr;
            iparm[i] = 0;
        }

        iparm[0] = 1;
        iparm[1] = 2;
        iparm[2] = 0;
        iparm[3] = 0;
        iparm[4] = 0;
        iparm[5] = 0;
        iparm[6] = 0;
        iparm[7] = 2;
        iparm[8] = 0;
        iparm[9] = 13;
        iparm[10] = 1;
        iparm[11] = 0;
        iparm[12] = 1;
        iparm[13] = 0;
        iparm[14] = 0;
        iparm[17] = -1;
        iparm[18] = -1;
        iparm[19] = 0;

        // 三阶段求解
        phase = 11;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        phase = 22;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        phase = 33;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, b.data(), x.data(), &error);

        if (error == 0) {
            print_vector("解向量x", x.data(), n);
        }

        phase = -1;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);
    }

    // 示例3: 使用稀疏矩阵描述符的LU分解
    cout << "\n=== 3. 稀疏LU分解 (使用稀疏矩阵描述符) ===" << endl;
    {
        const int n = 4;

        // 创建稀疏矩阵
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
            cout << "稀疏矩阵创建成功" << endl;

            // 创建LU分解（这里只是示例，实际LU分解需要更多步骤）
            // MKL的稀疏LU分解通常通过Pardiso完成

            vector<double> b(n, 1.0);
            vector<double> x(n, 0.0);

            // 使用Pardiso进行LU分解和求解
            void* pt[64];
            int iparm[64];
            int maxfct = 1, mnum = 1;
            int mtype = 11;  // 非对称矩阵
            int phase;
            int nrhs = 1, msglvl = 0, error = 0;

            for (int i = 0; i < 64; ++i) {
                pt[i] = nullptr;
                iparm[i] = 0;
            }

            iparm[0] = 1;
            iparm[1] = 2;
            iparm[2] = 0;

            // 转换为Fortran索引
            vector<int> ia(n + 1);
            vector<int> ja;
            vector<double> a;

            ia[0] = 1;  // Fortran索引从1开始
            for (int i = 0; i < n; ++i) {
                for (int j = row_ptr[i]; j < row_ptr[i+1]; ++j) {
                    ja.push_back(col_idx[j] + 1);  // Fortran索引
                    a.push_back(values[j]);
                }
                ia[i+1] = ja.size() + 1;
            }

            phase = 11;
            pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                    a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                    iparm, &msglvl, nullptr, nullptr, &error);

            phase = 22;
            pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                    a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                    iparm, &msglvl, nullptr, nullptr, &error);

            phase = 33;
            pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                    a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                    iparm, &msglvl, b.data(), x.data(), &error);

            if (error == 0) {
                print_vector("LU分解求解结果x", x.data(), n);
            }

            phase = -1;
            pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                    nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                    iparm, &msglvl, nullptr, nullptr, &error);
        }

        mkl_sparse_destroy(A);
    }

    // 示例4: 稀疏Cholesky分解（对称正定矩阵）
    cout << "\n=== 4. 稀疏Cholesky分解 ===" << endl;
    {
        const int n = 4;

        // 创建一个对称正定矩阵
        vector<int> ia = {1, 3, 5, 7, 9};
        vector<int> ja = {1, 2, 2, 3, 3, 4, 1, 4, 4};
        vector<double> a = {4.0, 1.0, 5.0, 2.0, 6.0, 3.0, 1.0, 3.0, 7.0};
        vector<double> b(n, 1.0);
        vector<double> x(n, 0.0);

        print_sparse_info("矩阵A (对称正定)", n, ja.size());

        void* pt[64];
        int iparm[64];
        int maxfct = 1, mnum = 1;
        int mtype = 2;  // 对称正定矩阵（使用Cholesky）
        int phase;
        int nrhs = 1, msglvl = 0, error = 0;

        for (int i = 0; i < 64; ++i) {
            pt[i] = nullptr;
            iparm[i] = 0;
        }

        iparm[0] = 1;
        iparm[1] = 2;
        iparm[2] = 0;

        phase = 11;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        phase = 22;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        phase = 33;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, b.data(), x.data(), &error);

        if (error == 0) {
            cout << "Cholesky分解求解完成" << endl;
            print_vector("解向量x", x.data(), n);
        }

        phase = -1;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);
    }

    // 示例5: 多次求解（重用分解）
    cout << "\n=== 5. 多次求解（重用分解） ===" << endl;
    {
        const int n = 3;
        const int num_rhs = 3;  // 多个右端向量

        vector<int> ia = {1, 3, 5, 7};
        vector<int> ja = {1, 2, 2, 3, 1, 3, 3};
        vector<double> a = {2.0, 1.0, 3.0, 1.0, 1.0, 4.0, 5.0};

        vector<double> b(n * num_rhs);
        vector<double> x(n * num_rhs, 0.0);

        // 创建多个右端向量
        for (int i = 0; i < num_rhs; ++i) {
            for (int j = 0; j < n; ++j) {
                b[i * n + j] = (double)(i + 1) * (j + 1);
            }
        }

        void* pt[64];
        int iparm[64];
        int maxfct = 1, mnum = 1;
        int mtype = 11;
        int phase;
        int nrhs = num_rhs, msglvl = 0, error = 0;

        for (int i = 0; i < 64; ++i) {
            pt[i] = nullptr;
            iparm[i] = 0;
        }

        iparm[0] = 1;
        iparm[1] = 2;
        iparm[2] = 0;

        // 阶段1和2：只执行一次
        phase = 11;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        phase = 22;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);

        // 阶段3：可以多次调用，使用不同的右端向量
        phase = 33;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                a.data(), ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, b.data(), x.data(), &error);

        if (error == 0) {
            cout << "多个右端向量求解完成" << endl;
            for (int i = 0; i < num_rhs; ++i) {
                cout << "右端向量 " << (i+1) << " 的解:" << endl;
                print_vector("x", x.data() + i*n, n, n);
            }
        }

        phase = -1;
        pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
                nullptr, ia.data(), ja.data(), nullptr, &nrhs,
                iparm, &msglvl, nullptr, nullptr, &error);
    }

    cin.get();
    return 0;
}
