#include <iostream>
#include <vector>
#include <chrono>
#include <complex>
#include <iomanip>

// Intel MKL BLAS头文件
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_matrix(const char* name, const double* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i * cols + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void print_vector(const char* name, const double* vector, int size) {
    cout << name << ":" << endl;
    for (int i = 0; i < size; ++i) {
        cout << vector[i] << " ";
    }
    cout << endl << endl;
}

void print_complex_matrix(const char* name, const MKL_Complex16* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = i * cols + j;
            cout << "(" << matrix[idx].real << ", " << matrix[idx].imag << ")\t";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL BLAS示例 ===" << endl;
    cout << "BLAS模块包括：" << endl;
    cout << "  - Level 1: 向量运算（点积、范数、缩放、向量加减、旋转等）" << endl;
    cout << "  - Level 2: 矩阵-向量运算（矩阵-向量乘法、对称矩阵-向量、三角矩阵-向量、rank更新等）" << endl;
    cout << "  - Level 3: 矩阵-矩阵运算（矩阵乘法、对称矩阵乘法、三角矩阵乘法、rank更新等）" << endl;
    cout << "  - 性能优化：SIMD向量化、多线程并行、内存对齐、缓存优化" << endl;
    cout << "  - 数据类型：单精度(s)、双精度(d)、复数单精度(c)、复数双精度(z)" << endl << endl;
    mkl_set_num_threads(4);
    // 设置矩阵维度
    const int m = 3, n = 3, k = 3;

    // 创建矩阵数据
    vector<double> A = {1.0, 2.0, 3.0,
                        4.0, 5.0, 6.0,
                        7.0, 8.0, 9.0};  // m x k

    vector<double> B = {2.0, 0.0, 1.0,
                        1.0, 1.0, 0.0,
                        0.0, 1.0, 2.0};  // k x n

    vector<double> C(m * n, 0.0);        // m x n，结果矩阵

    print_matrix("矩阵A", A.data(), m, k);
    print_matrix("矩阵B", B.data(), k, n);

    // ========== BLAS Level 3: 矩阵-矩阵运算 ==========
    cout << "\n=== BLAS Level 3: 矩阵-矩阵运算 ===" << endl;

    // 1. 矩阵乘法 dgemm
    cout << "\n--- 1. 矩阵乘法 (dgemm) ---" << endl;
    // C = alpha * A * B + beta * C
    double alpha = 1.0, beta = 0.0;

    auto start = high_resolution_clock::now();

    cblas_dgemm(CblasRowMajor,     // 行主序存储
                CblasNoTrans,       // A不转置
                CblasNoTrans,       // B不转置
                m, n, k,           // 矩阵维度
                alpha,             // 缩放因子
                A.data(), k,       // A矩阵和leading dimension
                B.data(), n,       // B矩阵和leading dimension
                beta,              // 结果矩阵缩放因子
                C.data(), n);      // C矩阵和leading dimension

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    print_matrix("A * B", C.data(), m, n);
    cout << "矩阵乘法耗时: " << duration.count() << " 微秒" << endl;

    // 2. 对称矩阵乘法 dsymm
    cout << "\n--- 2. 对称矩阵乘法 (dsymm) ---" << endl;
    const int n_symm = 3;
    vector<double> A_symm = {1.0, 2.0, 3.0,
                             2.0, 4.0, 5.0,
                             3.0, 5.0, 6.0};  // 对称矩阵
    vector<double> B_symm = {1.0, 0.0, 1.0,
                             0.0, 1.0, 0.0,
                             1.0, 0.0, 1.0};
    vector<double> C_symm(n_symm * n_symm, 0.0);

    print_matrix("对称矩阵A_symm", A_symm.data(), n_symm, n_symm);
    print_matrix("矩阵B_symm", B_symm.data(), n_symm, n_symm);

    cblas_dsymm(CblasRowMajor,     // 行主序
                CblasLeft,          // A在左边
                CblasUpper,         // 上三角存储
                n_symm, n_symm,   // 矩阵维度
                1.0,               // alpha
                A_symm.data(), n_symm, // A矩阵
                B_symm.data(), n_symm, // B矩阵
                0.0,               // beta
                C_symm.data(), n_symm); // C矩阵

    print_matrix("A_symm * B_symm", C_symm.data(), n_symm, n_symm);

    // 3. 三角矩阵乘法 dtrmm
    cout << "\n--- 3. 三角矩阵乘法 (dtrmm) ---" << endl;
    vector<double> A_trmm = {1.0, 2.0, 3.0,
                             0.0, 4.0, 5.0,
                             0.0, 0.0, 6.0};  // 上三角矩阵
    vector<double> B_trmm = {1.0, 0.0, 1.0,
                             0.0, 1.0, 0.0,
                             1.0, 0.0, 1.0};
    vector<double> C_trmm = B_trmm;  // 结果会覆盖B

    print_matrix("上三角矩阵A_trmm", A_trmm.data(), n_symm, n_symm);
    print_matrix("矩阵B_trmm (原始)", B_trmm.data(), n_symm, n_symm);

    cblas_dtrmm(CblasRowMajor,     // 行主序
                CblasLeft,          // A在左边
                CblasUpper,         // 上三角
                CblasNoTrans,       // 不转置
                CblasNonUnit,       // 非单位三角矩阵
                n_symm, n_symm,   // 矩阵维度
                1.0,               // alpha
                A_trmm.data(), n_symm, // A矩阵
                C_trmm.data(), n_symm); // B矩阵（结果）

    print_matrix("A_trmm * B_trmm", C_trmm.data(), n_symm, n_symm);

    // 4. 矩阵转置示例（使用dgemm）
    cout << "\n--- 4. 矩阵转置示例 (使用dgemm) ---" << endl;
    vector<double> A_T(k * m, 0.0);
    // 创建单位矩阵
    vector<double> I(m * m, 0.0);
    for (int i = 0; i < m; ++i) {
        I[i * m + i] = 1.0;
    }
    // 使用 dgemm 计算 A^T = A^T * I
    cblas_dgemm(CblasRowMajor,
                CblasTrans,        // A转置
                CblasNoTrans,      // I不转置
                k, m, m,          // 结果维度 k x m
                1.0,              // alpha
                A.data(), k,       // A矩阵
                I.data(), m,       // 单位矩阵
                0.0,              // beta
                A_T.data(), m);    // 结果矩阵
    print_matrix("A^T (使用dgemm)", A_T.data(), k, m);

    // ========== BLAS Level 2: 矩阵-向量运算 ==========
    cout << "\n=== BLAS Level 2: 矩阵-向量运算 ===" << endl;

    // 1. 矩阵-向量乘法 dgemv
    cout << "\n--- 1. 矩阵-向量乘法 (dgemv) ---" << endl;
    vector<double> x = {1.0, 2.0, 3.0};  // 向量
    vector<double> y(m, 0.0);            // 结果向量

    print_vector("向量x", x.data(), k);

    cblas_dgemv(CblasRowMajor,     // 行主序存储
                CblasNoTrans,       // A不转置
                m, k,              // 矩阵维度
                1.0,               // 缩放因子
                A.data(), k,       // A矩阵和leading dimension
                x.data(), 1,       // x向量和步长
                0.0,               // y的缩放因子
                y.data(), 1);      // y向量和步长

    print_vector("A * x", y.data(), m);

    // 2. 对称矩阵-向量乘法 dsymv
    cout << "\n--- 2. 对称矩阵-向量乘法 (dsymv) ---" << endl;
    const int n_sym = 3;
    vector<double> A_sym = {1.0, 2.0, 3.0,
                           2.0, 4.0, 5.0,
                           3.0, 5.0, 6.0};  // 对称矩阵（只存储上三角）
    vector<double> x_sym = {1.0, 2.0, 3.0};
    vector<double> y_sym(n_sym, 0.0);

    print_matrix("对称矩阵A_sym", A_sym.data(), n_sym, n_sym);
    print_vector("向量x_sym", x_sym.data(), n_sym);

    cblas_dsymv(CblasRowMajor,     // 行主序
                CblasUpper,         // 上三角存储
                n_sym,             // 矩阵维度
                1.0,               // alpha
                A_sym.data(), n_sym, // A矩阵
                x_sym.data(), 1,   // x向量
                0.0,               // beta
                y_sym.data(), 1);  // y向量

    print_vector("A_sym * x_sym", y_sym.data(), n_sym);

    // 3. 三角矩阵-向量乘法 dtrmv
    cout << "\n--- 3. 三角矩阵-向量乘法 (dtrmv) ---" << endl;
    vector<double> A_tri = {1.0, 2.0, 3.0,
                            0.0, 4.0, 5.0,
                            0.0, 0.0, 6.0};  // 上三角矩阵
    vector<double> x_tri = {1.0, 2.0, 3.0};
    vector<double> x_tri_result = x_tri;  // 结果会覆盖输入

    print_matrix("上三角矩阵A_tri", A_tri.data(), n_sym, n_sym);
    print_vector("向量x_tri (原始)", x_tri.data(), n_sym);

    cblas_dtrmv(CblasRowMajor,     // 行主序
                CblasUpper,         // 上三角
                CblasNoTrans,       // 不转置
                CblasNonUnit,       // 非单位三角矩阵
                n_sym,             // 矩阵维度
                A_tri.data(), n_sym, // A矩阵
                x_tri_result.data(), 1); // x向量（结果）

    print_vector("A_tri * x_tri", x_tri_result.data(), n_sym);

    // 4. 矩阵-向量外积 dger: A = alpha * x * y^T + A
    cout << "\n--- 4. 矩阵-向量外积 (dger) ---" << endl;
    vector<double> x_ger = {1.0, 2.0, 3.0};
    vector<double> y_ger = {2.0, 3.0};
    vector<double> A_ger(3 * 2, 0.0);  // 3x2矩阵
    print_vector("向量x_ger", x_ger.data(), 3);
    print_vector("向量y_ger", y_ger.data(), 2);
    cblas_dger(CblasRowMajor, 3, 2, 1.0, x_ger.data(), 1, y_ger.data(), 1, A_ger.data(), 2);
    print_matrix("外积 A = x * y^T", A_ger.data(), 3, 2);

    // 5. 三角矩阵求解 dtrsv: 解 A*x = b 或 A^T*x = b
    cout << "\n--- 5. 三角矩阵求解 (dtrsv) ---" << endl;
    vector<double> A_trsv = {2.0, 1.0, 3.0,
                              0.0, 3.0, 2.0,
                              0.0, 0.0, 4.0};  // 上三角矩阵
    vector<double> b_trsv = {8.0, 11.0, 12.0};  // 右端向量
    vector<double> x_trsv = b_trsv;  // 结果会覆盖输入
    print_matrix("上三角矩阵A_trsv", A_trsv.data(), 3, 3);
    print_vector("右端向量b_trsv", b_trsv.data(), 3);
    cblas_dtrsv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit,
                3, A_trsv.data(), 3, x_trsv.data(), 1);
    print_vector("解向量 x = A\\b", x_trsv.data(), 3);

    // 6. 对称rank-1更新 dsyr: A = alpha * x * x^T + A
    cout << "\n--- 6. 对称rank-1更新 (dsyr) ---" << endl;
    vector<double> A_syr = {1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0};  // 单位矩阵（对称）
    vector<double> x_syr = {1.0, 2.0, 3.0};
    print_matrix("原始对称矩阵A_syr", A_syr.data(), 3, 3);
    print_vector("向量x_syr", x_syr.data(), 3);
    cblas_dsyr(CblasRowMajor, CblasUpper, 3, 1.0, x_syr.data(), 1, A_syr.data(), 3);
    print_matrix("更新后 A = A + x*x^T", A_syr.data(), 3, 3);

    // 7. 对称rank-2更新 dsyr2: A = alpha * (x*y^T + y*x^T) + A
    cout << "\n--- 7. 对称rank-2更新 (dsyr2) ---" << endl;
    vector<double> A_syr2 = {1.0, 0.0, 0.0,
                             0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0};
    vector<double> x_syr2 = {1.0, 2.0, 3.0};
    vector<double> y_syr2 = {2.0, 1.0, 1.0};
    print_matrix("原始对称矩阵A_syr2", A_syr2.data(), 3, 3);
    print_vector("向量x_syr2", x_syr2.data(), 3);
    print_vector("向量y_syr2", y_syr2.data(), 3);
    cblas_dsyr2(CblasRowMajor, CblasUpper, 3, 1.0,
                x_syr2.data(), 1, y_syr2.data(), 1, A_syr2.data(), 3);
    print_matrix("更新后 A = A + x*y^T + y*x^T", A_syr2.data(), 3, 3);

    // ========== BLAS Level 1: 向量运算 ==========
    cout << "\n=== BLAS Level 1: 向量运算 ===" << endl;

    // 1. 向量点积 ddot
    cout << "\n--- 1. 向量点积 (ddot) ---" << endl;
    double dot_result = cblas_ddot(k, x.data(), 1, x.data(), 1);
    cout << "x · x = " << dot_result << endl;

    // 2. 向量范数 dnrm2
    cout << "\n--- 2. 向量范数 (dnrm2) ---" << endl;
    double norm_result = cblas_dnrm2(k, x.data(), 1);
    cout << "||x||_2 = " << norm_result << endl;

    // 3. 向量缩放 dscal: x = alpha * x
    cout << "\n--- 3. 向量缩放 (dscal) ---" << endl;
    vector<double> x_scaled = x;
    print_vector("原始向量x", x_scaled.data(), k);
    cblas_dscal(k, 2.5, x_scaled.data(), 1);
    print_vector("缩放后 2.5*x", x_scaled.data(), k);

    // 4. 向量加法 daxpy: y = alpha * x + y
    cout << "\n--- 4. 向量加法 (daxpy) ---" << endl;
    vector<double> z = {1.0, 1.0, 1.0};
    print_vector("向量z (初始)", z.data(), k);
    cblas_daxpy(k, 2.0, x.data(), 1, z.data(), 1);
    print_vector("2*x + z", z.data(), k);

    // 5. 向量复制 dcopy
    cout << "\n--- 5. 向量复制 (dcopy) ---" << endl;
    vector<double> x_copy(k);
    cblas_dcopy(k, x.data(), 1, x_copy.data(), 1);
    print_vector("x的副本", x_copy.data(), k);

    // 6. 最大绝对值索引 idamax
    cout << "\n--- 6. 最大绝对值索引 (idamax) ---" << endl;
    vector<double> x_max = {-3.0, 2.0, 5.0, -1.0, 4.0};
    int max_idx = cblas_idamax(5, x_max.data(), 1);
    cout << "向量: ";
    for (int i = 0; i < 5; ++i) cout << x_max[i] << " ";
    cout << endl;
    cout << "最大绝对值元素的索引: " << max_idx << " (值: " << x_max[max_idx] << ")" << endl << endl;

    // 7. 向量交换 dswap
    cout << "\n--- 7. 向量交换 (dswap) ---" << endl;
    vector<double> x_swap1 = {1.0, 2.0, 3.0};
    vector<double> x_swap2 = {4.0, 5.0, 6.0};
    print_vector("交换前 x_swap1", x_swap1.data(), 3);
    print_vector("交换前 x_swap2", x_swap2.data(), 3);
    cblas_dswap(3, x_swap1.data(), 1, x_swap2.data(), 1);
    print_vector("交换后 x_swap1", x_swap1.data(), 3);
    print_vector("交换后 x_swap2", x_swap2.data(), 3);

    // 8. 绝对值求和 dasum
    cout << "\n--- 8. 绝对值求和 (dasum) ---" << endl;
    vector<double> x_asum = {-1.0, 2.0, -3.0, 4.0, -5.0};
    double asum_result = cblas_dasum(5, x_asum.data(), 1);
    print_vector("向量x_asum", x_asum.data(), 5);
    cout << "绝对值求和: " << asum_result << endl << endl;

    // 9. 最小绝对值索引 idamin
    cout << "\n--- 9. 最小绝对值索引 (idamin) ---" << endl;
    vector<double> x_min = {-3.0, 2.0, 5.0, -1.0, 4.0};
    int min_idx = cblas_idamin(5, x_min.data(), 1);
    cout << "向量: ";
    for (int i = 0; i < 5; ++i) cout << x_min[i] << " ";
    cout << endl;
    cout << "最小绝对值元素的索引: " << min_idx << " (值: " << x_min[min_idx] << ")" << endl << endl;

    // 10. 向量旋转 drot
    cout << "\n--- 10. 向量旋转 (drot) ---" << endl;
    vector<double> x_rot = {1.0, 2.0};
    vector<double> y_rot = {3.0, 4.0};
    double c = 0.6;  // cos(角度)
    double s = 0.8;  // sin(角度)
    print_vector("旋转前 x_rot", x_rot.data(), 2);
    print_vector("旋转前 y_rot", y_rot.data(), 2);
    cblas_drot(2, x_rot.data(), 1, y_rot.data(), 1, c, s);
    print_vector("旋转后 x_rot", x_rot.data(), 2);
    print_vector("旋转后 y_rot", y_rot.data(), 2);

    // ========== 不同数据类型的示例 ==========
    cout << "\n=== 不同数据类型的示例 ===" << endl;

    // 1. 单精度 (float) - sgemm
    cout << "\n--- 1. 单精度矩阵乘法 (sgemm) ---" << endl;
    {
        const int m_f = 2, n_f = 2, k_f = 2;
        vector<float> A_f = {1.0f, 2.0f, 3.0f, 4.0f};
        vector<float> B_f = {2.0f, 0.0f, 1.0f, 2.0f};
        vector<float> C_f(m_f * n_f, 0.0f);

        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m_f, n_f, k_f, 1.0f,
                    A_f.data(), k_f, B_f.data(), n_f,
                    0.0f, C_f.data(), n_f);

        cout << "单精度矩阵乘法结果:" << endl;
        for (int i = 0; i < m_f; ++i) {
            for (int j = 0; j < n_f; ++j) {
                cout << C_f[i * n_f + j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    // 2. 复数单精度 (complex<float>) - cgemm
    cout << "\n--- 2. 复数单精度矩阵乘法 (cgemm) ---" << endl;
    {
        const int m_c = 2, n_c = 2, k_c = 2;
        vector<MKL_Complex8> A_c(m_c * k_c);
        vector<MKL_Complex8> B_c(k_c * n_c);
        vector<MKL_Complex8> C_c(m_c * n_c);

        // 初始化复数矩阵
        A_c[0].real = 1.0f; A_c[0].imag = 1.0f;
        A_c[1].real = 2.0f; A_c[1].imag = 0.0f;
        A_c[2].real = 3.0f; A_c[2].imag = -1.0f;
        A_c[3].real = 4.0f; A_c[3].imag = 2.0f;

        B_c[0].real = 1.0f; B_c[0].imag = 0.0f;
        B_c[1].real = 0.0f; B_c[1].imag = 1.0f;
        B_c[2].real = 1.0f; B_c[2].imag = 1.0f;
        B_c[3].real = 2.0f; B_c[3].imag = 0.0f;

        MKL_Complex8 alpha_c, beta_c;
        alpha_c.real = 1.0f; alpha_c.imag = 0.0f;
        beta_c.real = 0.0f; beta_c.imag = 0.0f;

        cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m_c, n_c, k_c, &alpha_c,
                    A_c.data(), k_c, B_c.data(), n_c,
                    &beta_c, C_c.data(), n_c);

        cout << "复数单精度矩阵乘法结果:" << endl;
        for (int i = 0; i < m_c; ++i) {
            for (int j = 0; j < n_c; ++j) {
                int idx = i * n_c + j;
                cout << "(" << C_c[idx].real << ", " << C_c[idx].imag << ")\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    // 3. 复数双精度 (complex<double>) - zgemm
    cout << "\n--- 3. 复数双精度矩阵乘法 (zgemm) ---" << endl;
    {
        const int m_z = 2, n_z = 2, k_z = 2;
        vector<MKL_Complex16> A_z(m_z * k_z);
        vector<MKL_Complex16> B_z(k_z * n_z);
        vector<MKL_Complex16> C_z(m_z * n_z);

        // 初始化复数矩阵
        A_z[0].real = 1.0; A_z[0].imag = 1.0;
        A_z[1].real = 2.0; A_z[1].imag = 0.0;
        A_z[2].real = 3.0; A_z[2].imag = -1.0;
        A_z[3].real = 4.0; A_z[3].imag = 2.0;

        B_z[0].real = 1.0; B_z[0].imag = 0.0;
        B_z[1].real = 0.0; B_z[1].imag = 1.0;
        B_z[2].real = 1.0; B_z[2].imag = 1.0;
        B_z[3].real = 2.0; B_z[3].imag = 0.0;

        MKL_Complex16 alpha_z, beta_z;
        alpha_z.real = 1.0; alpha_z.imag = 0.0;
        beta_z.real = 0.0; beta_z.imag = 0.0;

        cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m_z, n_z, k_z, &alpha_z,
                    A_z.data(), k_z, B_z.data(), n_z,
                    &beta_z, C_z.data(), n_z);

        cout << "复数矩阵乘法结果:" << endl;
        for (int i = 0; i < m_z; ++i) {
            for (int j = 0; j < n_z; ++j) {
                int idx = i * n_z + j;
                cout << "(" << C_z[idx].real << ", " << C_z[idx].imag << ")\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    // ========== 性能优化示例 ==========
    cout << "\n=== 性能优化示例 ===" << endl;

    // 1. 内存对齐的重要性
    cout << "\n--- 1. 内存对齐的重要性 ---" << endl;
    {
        const int n_perf = 1000;
        const int iterations = 1000;

        // 未对齐内存
        vector<double> unaligned_data(n_perf);
        for (int i = 0; i < n_perf; ++i) {
            unaligned_data[i] = i;
        }

        // 对齐内存（64字节对齐）
        double* aligned_data = (double*)mkl_malloc(n_perf * sizeof(double), 64);
        for (int i = 0; i < n_perf; ++i) {
            aligned_data[i] = i;
        }

        // 测试未对齐内存的性能
        auto start_unaligned = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            double result = cblas_ddot(n_perf, unaligned_data.data(), 1, unaligned_data.data(), 1);
        }
        auto end_unaligned = high_resolution_clock::now();
        auto duration_unaligned = duration_cast<microseconds>(end_unaligned - start_unaligned);

        // 测试对齐内存的性能
        auto start_aligned = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            double result = cblas_ddot(n_perf, aligned_data, 1, aligned_data, 1);
        }
        auto end_aligned = high_resolution_clock::now();
        auto duration_aligned = duration_cast<microseconds>(end_aligned - start_aligned);

        cout << "未对齐内存耗时: " << duration_unaligned.count() << " 微秒" << endl;
        cout << "对齐内存耗时: " << duration_aligned.count() << " 微秒" << endl;
        cout << "性能提升: " << (double)duration_unaligned.count() / duration_aligned.count() << "x" << endl;
        cout << "注意：对齐内存可以提高SIMD操作的性能" << endl << endl;

        mkl_free(aligned_data);
    }

    // 2. 多线程性能
    cout << "\n--- 2. 多线程性能 ---" << endl;
    {
        const int n_threads = mkl_get_max_threads();
        cout << "当前MKL最大线程数: " << n_threads << endl;
        cout << "MKL自动利用多线程加速矩阵运算" << endl;
        cout << "可以通过 mkl_set_num_threads() 设置线程数" << endl << endl;

        // 测试大规模矩阵乘法
        const int n_large = 500;
        vector<double> A_large(n_large * n_large);
        vector<double> B_large(n_large * n_large);
        vector<double> C_large(n_large * n_large, 0.0);

        // 初始化
        for (int i = 0; i < n_large * n_large; ++i) {
            A_large[i] = i * 0.001;
            B_large[i] = i * 0.001;
        }

        auto start_large = high_resolution_clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    n_large, n_large, n_large, 1.0,
                    A_large.data(), n_large, B_large.data(), n_large,
                    0.0, C_large.data(), n_large);
        auto end_large = high_resolution_clock::now();
        auto duration_large = duration_cast<milliseconds>(end_large - start_large);

        cout << "大规模矩阵乘法 (" << n_large << "x" << n_large << ") 耗时: "
             << duration_large.count() << " 毫秒" << endl;
        cout << "MKL自动利用多核CPU和SIMD指令优化性能" << endl << endl;
    }

    // 3. 性能对比：syrk vs gemm
    cout << "\n--- 3. 性能对比：syrk vs gemm ---" << endl;
    {
        const int n_syrk = 3, k_syrk = 2;
        vector<double> A_syrk = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // 3x2矩阵
        vector<double> C_syrk(n_syrk * n_syrk, 0.0);  // 3x3对称矩阵

        print_matrix("矩阵A_syrk (3x2)", A_syrk.data(), n_syrk, k_syrk);

        cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n_syrk, k_syrk, 1.0,
                    A_syrk.data(), k_syrk, 0.0, C_syrk.data(), n_syrk);

        print_matrix("对称矩阵 C = A * A^T (使用dsyrk)", C_syrk.data(), n_syrk, n_syrk);

        // 对比：使用dgemm计算相同结果
        vector<double> C_gemm(n_syrk * n_syrk, 0.0);
        auto start_gemm = high_resolution_clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    n_syrk, n_syrk, k_syrk, 1.0,
                    A_syrk.data(), k_syrk, A_syrk.data(), k_syrk,
                    0.0, C_gemm.data(), n_syrk);
        auto end_gemm = high_resolution_clock::now();
        auto duration_gemm = duration_cast<nanoseconds>(end_gemm - start_gemm);

        auto start_syrk = high_resolution_clock::now();
        cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n_syrk, k_syrk, 1.0,
                    A_syrk.data(), k_syrk, 0.0, C_syrk.data(), n_syrk);
        auto end_syrk = high_resolution_clock::now();
        auto duration_syrk = duration_cast<nanoseconds>(end_syrk - start_syrk);

        cout << "dgemm耗时: " << duration_gemm.count() << " 纳秒" << endl;
        cout << "dsyrk耗时: " << duration_syrk.count() << " 纳秒" << endl;
        if (duration_gemm.count() > 0) {
            cout << "性能提升: " << (double)duration_gemm.count() / duration_syrk.count() << "x" << endl;
        }
        cout << "注意：dsyrk专门用于对称矩阵，比使用dgemm更高效" << endl << endl;
    }

    // 4. 三角矩阵求解 dtrsm: 解 A*X = alpha*B 或 X*A = alpha*B
    cout << "\n--- 4. 三角矩阵求解 (dtrsm) ---" << endl;
    {
        const int n_trsm = 3;
        vector<double> A_trsm = {2.0, 1.0, 3.0,
                                 0.0, 3.0, 2.0,
                                 0.0, 0.0, 4.0};  // 上三角矩阵
        vector<double> B_trsm = {4.0, 2.0, 1.0,
                                 6.0, 3.0, 2.0,
                                 8.0, 4.0, 3.0};  // 右端矩阵
        vector<double> X_trsm = B_trsm;  // 结果会覆盖B

        print_matrix("上三角矩阵A_trsm", A_trsm.data(), n_trsm, n_trsm);
        print_matrix("右端矩阵B_trsm", B_trsm.data(), n_trsm, n_trsm);

        cblas_dtrsm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit,
                    n_trsm, n_trsm, 1.0, A_trsm.data(), n_trsm, X_trsm.data(), n_trsm);

        print_matrix("解矩阵 X = A\\B", X_trsm.data(), n_trsm, n_trsm);
        cout << "注意：dtrsm用于求解多个线性方程组" << endl << endl;
    }

    // 5. 对称rank-2k更新 dsyr2k: C = alpha * (A*B^T + B*A^T) + beta * C
    cout << "\n--- 5. 对称rank-2k更新 (dsyr2k) ---" << endl;
    {
        const int n_syr2k = 3, k_syr2k = 2;
        vector<double> A_syr2k = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // 3x2
        vector<double> B_syr2k = {2.0, 1.0, 1.0, 2.0, 3.0, 1.0};  // 3x2
        vector<double> C_syr2k(n_syr2k * n_syr2k, 0.0);

        print_matrix("矩阵A_syr2k (3x2)", A_syr2k.data(), n_syr2k, k_syr2k);
        print_matrix("矩阵B_syr2k (3x2)", B_syr2k.data(), n_syr2k, k_syr2k);

        cblas_dsyr2k(CblasRowMajor, CblasUpper, CblasNoTrans,
                     n_syr2k, k_syr2k, 1.0,
                     A_syr2k.data(), k_syr2k, B_syr2k.data(), k_syr2k,
                     0.0, C_syr2k.data(), n_syr2k);

        print_matrix("对称矩阵 C = A*B^T + B*A^T", C_syr2k.data(), n_syr2k, n_syr2k);
        cout << "注意：dsyr2k用于对称矩阵的rank-2k更新" << endl << endl;
    }

    // 6. 带状矩阵示例
    cout << "\n--- 6. 带状矩阵运算示例 ---" << endl;
    {
        cout << "带状矩阵函数：" << endl;
        cout << "  - gbmv: 一般带状矩阵-向量乘法" << endl;
        cout << "  - sbmv: 对称带状矩阵-向量乘法" << endl;
        cout << "  - tbmv/tbsv: 三角带状矩阵-向量乘法/求解" << endl;
        cout << "带状矩阵存储格式：只存储非零带，节省内存" << endl << endl;

        // 示例：对称带状矩阵 sbmv
        const int n_band = 4, ku = 1, kl = 1;  // 带宽
        vector<double> A_band = {0.0, 1.0, 2.0, 3.0,  // 对称带状矩阵（打包格式）
                                 1.0, 2.0, 3.0, 4.0,
                                 2.0, 3.0, 4.0, 5.0};
        vector<double> x_band = {1.0, 2.0, 3.0, 4.0};
        vector<double> y_band(n_band, 0.0);

        print_vector("向量x_band", x_band.data(), n_band);
        cblas_dsbmv(CblasRowMajor, CblasUpper, n_band, ku,
                    1.0, A_band.data(), ku + 1, x_band.data(), 1,
                    0.0, y_band.data(), 1);
        print_vector("对称带状矩阵-向量乘法结果", y_band.data(), n_band);
    }

    // 7. 打包矩阵示例
    cout << "\n--- 7. 打包矩阵运算示例 ---" << endl;
    {
        cout << "打包矩阵函数：" << endl;
        cout << "  - spmv/spr/spr2: 对称打包矩阵-向量乘法/rank更新" << endl;
        cout << "  - tpmv/tpsv: 三角打包矩阵-向量乘法/求解" << endl;
        cout << "打包格式：只存储上三角或下三角，节省一半内存" << endl << endl;

        // 示例：对称打包矩阵 spmv
        const int n_pack = 3;
        vector<double> A_pack = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // 打包格式（上三角）
        vector<double> x_pack = {1.0, 2.0, 3.0};
        vector<double> y_pack(n_pack, 0.0);

        print_vector("向量x_pack", x_pack.data(), n_pack);
        cblas_dspmv(CblasRowMajor, CblasUpper, n_pack,
                    1.0, A_pack.data(), x_pack.data(), 1,
                    0.0, y_pack.data(), 1);
        print_vector("对称打包矩阵-向量乘法结果", y_pack.data(), n_pack);
    }

    // 8. 复数厄米矩阵示例
    cout << "\n--- 8. 复数厄米矩阵运算示例 ---" << endl;
    {
        cout << "复数厄米矩阵函数：" << endl;
        cout << "  Level 2: hemv, her, her2" << endl;
        cout << "  Level 3: hemm, herk, her2k" << endl;
        cout << "厄米矩阵：A = A^H (共轭转置等于自身)" << endl << endl;

        // 示例：厄米矩阵-向量乘法 zhemv
        const int n_herm = 2;
        vector<MKL_Complex16> A_herm(n_herm * n_herm);
        vector<MKL_Complex16> x_herm(n_herm);
        vector<MKL_Complex16> y_herm(n_herm);

        // 初始化厄米矩阵（上三角存储）
        A_herm[0].real = 1.0; A_herm[0].imag = 0.0;
        A_herm[1].real = 2.0; A_herm[1].imag = 1.0;
        A_herm[2].real = 0.0; A_herm[2].imag = 0.0;  // 下三角不存储
        A_herm[3].real = 3.0; A_herm[3].imag = 0.0;

        x_herm[0].real = 1.0; x_herm[0].imag = 0.0;
        x_herm[1].real = 2.0; x_herm[1].imag = 1.0;

        MKL_Complex16 alpha_herm, beta_herm;
        alpha_herm.real = 1.0; alpha_herm.imag = 0.0;
        beta_herm.real = 0.0; beta_herm.imag = 0.0;

        cblas_zhemv(CblasRowMajor, CblasUpper, n_herm,
                    &alpha_herm, A_herm.data(), n_herm,
                    x_herm.data(), 1, &beta_herm, y_herm.data(), 1);

        cout << "厄米矩阵-向量乘法结果:" << endl;
        for (int i = 0; i < n_herm; ++i) {
            cout << "(" << y_herm[i].real << ", " << y_herm[i].imag << ") ";
        }
        cout << endl << endl;

        // 示例：厄米rank-k更新 zherk
        const int n_herk = 2, k_herk = 2;
        vector<MKL_Complex16> A_herk(n_herk * k_herk);
        vector<MKL_Complex16> C_herk(n_herk * n_herk);

        A_herk[0].real = 1.0; A_herk[0].imag = 1.0;
        A_herk[1].real = 2.0; A_herk[1].imag = 0.0;
        A_herk[2].real = 3.0; A_herk[2].imag = -1.0;
        A_herk[3].real = 4.0; A_herk[3].imag = 2.0;

        cblas_zherk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n_herk, k_herk, 1.0, A_herk.data(), k_herk,
                    0.0, C_herk.data(), n_herk);

        print_complex_matrix("厄米rank-k更新 C = A*A^H", C_herk.data(), n_herk, n_herk);
    }

    // 9. 批量操作性能
    cout << "\n--- 9. 批量操作性能 ---" << endl;
    {
        const int batch_size = 100;
        const int n_batch = 10;
        vector<double> x_batch(n_batch);
        vector<double> y_batch(n_batch);

        for (int i = 0; i < n_batch; ++i) {
            x_batch[i] = i + 1;
            y_batch[i] = 0;
        }

        auto start_batch = high_resolution_clock::now();
        for (int b = 0; b < batch_size; ++b) {
            cblas_daxpy(n_batch, 1.0, x_batch.data(), 1, y_batch.data(), 1);
        }
        auto end_batch = high_resolution_clock::now();
        auto duration_batch = duration_cast<microseconds>(end_batch - start_batch);

        cout << "批量操作 (" << batch_size << " 次 daxpy) 耗时: "
             << duration_batch.count() << " 微秒" << endl;
        cout << "MKL的向量化实现使得批量操作非常高效" << endl << endl;
    }

    // 10. 缓存优化说明
    cout << "\n--- 10. 缓存优化 ---" << endl;
    {
        cout << "MKL BLAS针对CPU缓存进行了优化：" << endl;
        cout << "  - 分块算法：将大矩阵分成小块以适应L1/L2/L3缓存" << endl;
        cout << "  - 数据局部性：优化数据访问模式，减少缓存未命中" << endl;
        cout << "  - 预取：智能预取数据到缓存" << endl;
        cout << "  - 循环展开：减少循环开销，提高指令级并行度" << endl;
        cout << "这些优化使得MKL BLAS在大规模矩阵运算中性能优异" << endl << endl;

        // 演示不同矩阵大小的性能
        const int sizes[] = {100, 200, 500, 1000};
        cout << "不同矩阵大小的性能对比 (dgemm):" << endl;
        for (int s : sizes) {
            vector<double> A_cache(s * s);
            vector<double> B_cache(s * s);
            vector<double> C_cache(s * s, 0.0);

            // 初始化
            for (int i = 0; i < s * s; ++i) {
                A_cache[i] = i * 0.001;
                B_cache[i] = i * 0.001;
            }

            auto start_cache = high_resolution_clock::now();
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                        s, s, s, 1.0,
                        A_cache.data(), s, B_cache.data(), s,
                        0.0, C_cache.data(), s);
            auto end_cache = high_resolution_clock::now();
            auto duration_cache = duration_cast<milliseconds>(end_cache - start_cache);

            if (duration_cache.count() > 0) {
                double gflops = (2.0 * s * s * s) / (duration_cache.count() * 1e-3)  / 1e9;  // GFLOPS计算
                cout << "  " << s << "x" << s << ": " << duration_cache.count()
                     << " 毫秒 (约 " << fixed << setprecision(2) << gflops << " GFLOPS)" << endl;
            } else {
                cout << "  " << s << "x" << s << ": " << duration_cache.count() << " 毫秒" << endl;
            }
        }
        cout << "注意：MKL自动选择最优的分块大小以适应缓存" << endl << endl;
    }

    // ========== BLAS vs 常规算法性能对比 ==========
    cout << "\n=== BLAS vs 常规算法性能对比 ===" << endl;
    cout << "本节对比MKL BLAS优化实现与常规C++实现的性能差异" << endl;
    cout << "测试规模：向量大小 1024×1024，矩阵大小 1024×1024" << endl << endl;

    // 1. Level 1: 向量点积 ddot
    cout << "\n--- 1. 向量点积 (ddot) 性能对比 ---" << endl;
    {
        const int n_dot = 1024 * 1024;  // 1024×1024 = 1,048,576
        const int iterations_dot = 50;
        vector<double> x_dot(n_dot);
        vector<double> y_dot(n_dot);

        for (int i = 0; i < n_dot; ++i) {
            x_dot[i] = i * 0.001;
            y_dot[i] = (i + 1) * 0.001;
        }

        // BLAS实现
        double result_blas = 0.0;
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_dot; ++iter) {
            result_blas = cblas_ddot(n_dot, x_dot.data(), 1, y_dot.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        double result_naive = 0.0;
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_dot; ++iter) {
            result_naive = 0.0;
            for (int i = 0; i < n_dot; ++i) {
                result_naive += x_dot[i] * y_dot[i];
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n_dot << ", 迭代次数: " << iterations_dot << endl;
        cout << "BLAS (ddot) 耗时: " << duration_blas.count() << " 微秒, 结果: " << result_blas << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒, 结果: " << result_naive << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：SIMD向量化、循环展开、内存预取" << endl << endl;
    }

    // 2. Level 1: 向量范数 dnrm2
    cout << "\n--- 2. 向量范数 (dnrm2) 性能对比 ---" << endl;
    {
        const int n_nrm = 1024 * 1024;  // 1024×1024
        const int iterations_nrm = 50;
        vector<double> x_nrm(n_nrm);

        for (int i = 0; i < n_nrm; ++i) {
            x_nrm[i] = i * 0.001;
        }

        // BLAS实现
        double result_blas = 0.0;
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_nrm; ++iter) {
            result_blas = cblas_dnrm2(n_nrm, x_nrm.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        double result_naive = 0.0;
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_nrm; ++iter) {
            result_naive = 0.0;
            for (int i = 0; i < n_nrm; ++i) {
                result_naive += x_nrm[i] * x_nrm[i];
            }
            result_naive = sqrt(result_naive);
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n_nrm << ", 迭代次数: " << iterations_nrm << endl;
        cout << "BLAS (dnrm2) 耗时: " << duration_blas.count() << " 微秒, 结果: " << result_blas << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒, 结果: " << result_naive << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：数值稳定性优化、避免溢出" << endl << endl;
    }

    // 3. Level 1: 向量缩放 dscal
    cout << "\n--- 3. 向量缩放 (dscal) 性能对比 ---" << endl;
    {
        const int n_scal = 1024 * 1024;  // 1024×1024
        const int iterations_scal = 50;
        vector<double> x_scal_blas(n_scal);
        vector<double> x_scal_naive(n_scal);
        double alpha_scal = 2.5;

        for (int i = 0; i < n_scal; ++i) {
            x_scal_blas[i] = i * 0.001;
            x_scal_naive[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_scal; ++iter) {
            // 恢复原始值
            for (int i = 0; i < n_scal; ++i) {
                x_scal_blas[i] = i * 0.001;
            }
            cblas_dscal(n_scal, alpha_scal, x_scal_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_scal; ++iter) {
            // 恢复原始值
            for (int i = 0; i < n_scal; ++i) {
                x_scal_naive[i] = i * 0.001;
            }
            for (int i = 0; i < n_scal; ++i) {
                x_scal_naive[i] *= alpha_scal;
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n_scal << ", 迭代次数: " << iterations_scal << endl;
        cout << "BLAS (dscal) 耗时: " << duration_blas.count() << " 微秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：SIMD并行处理多个元素" << endl << endl;
    }

    // 4. Level 1: 向量加法 daxpy
    cout << "\n--- 4. 向量加法 (daxpy) 性能对比 ---" << endl;
    {
        const int n_axpy = 1024 * 1024;  // 1024×1024
        const int iterations_axpy = 50;
        vector<double> x_axpy(n_axpy);
        vector<double> y_axpy_blas(n_axpy);
        vector<double> y_axpy_naive(n_axpy);
        double alpha_axpy = 2.0;

        for (int i = 0; i < n_axpy; ++i) {
            x_axpy[i] = i * 0.001;
            y_axpy_blas[i] = i * 0.0005;
            y_axpy_naive[i] = i * 0.0005;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_axpy; ++iter) {
            // 恢复原始值
            for (int i = 0; i < n_axpy; ++i) {
                y_axpy_blas[i] = i * 0.0005;
            }
            cblas_daxpy(n_axpy, alpha_axpy, x_axpy.data(), 1, y_axpy_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_axpy; ++iter) {
            // 恢复原始值
            for (int i = 0; i < n_axpy; ++i) {
                y_axpy_naive[i] = i * 0.0005;
            }
            for (int i = 0; i < n_axpy; ++i) {
                y_axpy_naive[i] = alpha_axpy * x_axpy[i] + y_axpy_naive[i];
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n_axpy << ", 迭代次数: " << iterations_axpy << endl;
        cout << "BLAS (daxpy) 耗时: " << duration_blas.count() << " 微秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：融合乘加指令(FMA)、向量化" << endl << endl;
    }

    // 5. Level 2: 矩阵-向量乘法 dgemv
    cout << "\n--- 5. 矩阵-向量乘法 (dgemv) 性能对比 ---" << endl;
    {
        const int m_gemv = 1024, n_gemv = 1024;  // 1024×1024
        const int iterations_gemv = 10;
        vector<double> A_gemv(m_gemv * n_gemv);
        vector<double> x_gemv(n_gemv);
        vector<double> y_gemv_blas(m_gemv);
        vector<double> y_gemv_naive(m_gemv);

        for (int i = 0; i < m_gemv * n_gemv; ++i) {
            A_gemv[i] = i * 0.001;
        }
        for (int i = 0; i < n_gemv; ++i) {
            x_gemv[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_gemv; ++iter) {
            for (int i = 0; i < m_gemv; ++i) {
                y_gemv_blas[i] = 0.0;
            }
            cblas_dgemv(CblasRowMajor, CblasNoTrans, m_gemv, n_gemv, 1.0,
                        A_gemv.data(), n_gemv, x_gemv.data(), 1, 0.0, y_gemv_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_gemv; ++iter) {
            for (int i = 0; i < m_gemv; ++i) {
                y_gemv_naive[i] = 0.0;
                for (int j = 0; j < n_gemv; ++j) {
                    y_gemv_naive[i] += A_gemv[i * n_gemv + j] * x_gemv[j];
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << m_gemv << "x" << n_gemv << ", 迭代次数: " << iterations_gemv << endl;
        cout << "BLAS (dgemv) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：缓存优化、分块算法、SIMD向量化" << endl << endl;
    }

    // 6. Level 3: 矩阵-矩阵乘法 dgemm
    cout << "\n--- 6. 矩阵-矩阵乘法 (dgemm) 性能对比 ---" << endl;
    {
        const int sizes[] = {256, 512, 1024};  // 包含1024×1024
        const int iterations_gemm = 3;

        for (int size_idx = 0; size_idx < 3; ++size_idx) {
            const int n_gemm = sizes[size_idx];
            vector<double> A_gemm(n_gemm * n_gemm);
            vector<double> B_gemm(n_gemm * n_gemm);
            vector<double> C_gemm_blas(n_gemm * n_gemm);
            vector<double> C_gemm_naive(n_gemm * n_gemm);

            // 初始化矩阵
            for (int i = 0; i < n_gemm * n_gemm; ++i) {
                A_gemm[i] = i * 0.001;
                B_gemm[i] = (i + 1) * 0.001;
            }

            // BLAS实现
            auto start_blas = high_resolution_clock::now();
            for (int iter = 0; iter < iterations_gemm; ++iter) {
                cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                            n_gemm, n_gemm, n_gemm, 1.0,
                            A_gemm.data(), n_gemm, B_gemm.data(), n_gemm,
                            0.0, C_gemm_blas.data(), n_gemm);
            }
            auto end_blas = high_resolution_clock::now();
            auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

            // 常规实现（三重循环）
            auto start_naive = high_resolution_clock::now();
            for (int iter = 0; iter < iterations_gemm; ++iter) {
                for (int i = 0; i < n_gemm; ++i) {
                    for (int j = 0; j < n_gemm; ++j) {
                        C_gemm_naive[i * n_gemm + j] = 0.0;
                        for (int k = 0; k < n_gemm; ++k) {
                            C_gemm_naive[i * n_gemm + j] += A_gemm[i * n_gemm + k] * B_gemm[k * n_gemm + j];
                        }
                    }
                }
            }
            auto end_naive = high_resolution_clock::now();
            auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

            cout << "矩阵大小: " << n_gemm << "x" << n_gemm << ", 迭代次数: " << iterations_gemm << endl;
            cout << "BLAS (dgemm) 耗时: " << duration_blas.count() << " 毫秒" << endl;
            cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
            if (duration_naive.count() > 0 && duration_blas.count() > 0) {
                double speedup = (double)duration_naive.count() / duration_blas.count();
                cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;

                // 计算GFLOPS
                double ops = 2.0 * n_gemm * n_gemm * n_gemm * iterations_gemm;
                double gflops_blas = ops / (duration_blas.count() * 1e-3) / 1e9;
                double gflops_naive = ops / (duration_naive.count() * 1e-3) / 1e9;
                cout << "BLAS性能: " << fixed << setprecision(2) << gflops_blas << " GFLOPS" << endl;
                cout << "常规实现性能: " << fixed << setprecision(2) << gflops_naive << " GFLOPS" << endl;
            }
            cout << "BLAS优势：分块算法、多线程并行、SIMD、缓存优化" << endl << endl;
        }
    }

    // 7. Level 2: 对称矩阵-向量乘法 dsymv
    cout << "\n--- 7. 对称矩阵-向量乘法 (dsymv) 性能对比 ---" << endl;
    {
        const int n_symv = 1024;  // 1024×1024对称矩阵
        const int iterations_symv = 10;
        vector<double> A_symv(n_symv * n_symv);
        vector<double> x_symv(n_symv);
        vector<double> y_symv_blas(n_symv);
        vector<double> y_symv_naive(n_symv);

        // 初始化对称矩阵（上三角存储）
        for (int i = 0; i < n_symv; ++i) {
            for (int j = i; j < n_symv; ++j) {
                double val = (i * n_symv + j) * 0.001;
                A_symv[i * n_symv + j] = val;
                A_symv[j * n_symv + i] = val;  // 对称
            }
        }
        for (int i = 0; i < n_symv; ++i) {
            x_symv[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_symv; ++iter) {
            cblas_dsymv(CblasRowMajor, CblasUpper, n_symv, 1.0,
                        A_symv.data(), n_symv, x_symv.data(), 1,
                        0.0, y_symv_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_symv; ++iter) {
            for (int i = 0; i < n_symv; ++i) {
                y_symv_naive[i] = 0.0;
                for (int j = 0; j < n_symv; ++j) {
                    y_symv_naive[i] += A_symv[i * n_symv + j] * x_symv[j];
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << n_symv << "x" << n_symv << " (对称), 迭代次数: " << iterations_symv << endl;
        cout << "BLAS (dsymv) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：利用对称性减少计算量、缓存优化" << endl << endl;
    }

    // 8. Level 3: 对称矩阵乘法 dsymm
    cout << "\n--- 8. 对称矩阵乘法 (dsymm) 性能对比 ---" << endl;
    {
        const int n_symm = 1024;  // 1024×1024
        const int iterations_symm = 3;
        vector<double> A_symm(n_symm * n_symm);
        vector<double> B_symm(n_symm * n_symm);
        vector<double> C_symm_blas(n_symm * n_symm);
        vector<double> C_symm_naive(n_symm * n_symm);

        // 初始化对称矩阵A
        for (int i = 0; i < n_symm; ++i) {
            for (int j = i; j < n_symm; ++j) {
                double val = (i * n_symm + j) * 0.001;
                A_symm[i * n_symm + j] = val;
                A_symm[j * n_symm + i] = val;
            }
        }
        // 初始化矩阵B
        for (int i = 0; i < n_symm * n_symm; ++i) {
            B_symm[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_symm; ++iter) {
            cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper,
                        n_symm, n_symm, 1.0,
                        A_symm.data(), n_symm, B_symm.data(), n_symm,
                        0.0, C_symm_blas.data(), n_symm);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_symm; ++iter) {
            for (int i = 0; i < n_symm; ++i) {
                for (int j = 0; j < n_symm; ++j) {
                    C_symm_naive[i * n_symm + j] = 0.0;
                    for (int k = 0; k < n_symm; ++k) {
                        C_symm_naive[i * n_symm + j] += A_symm[i * n_symm + k] * B_symm[k * n_symm + j];
                    }
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << n_symm << "x" << n_symm << " (A对称), 迭代次数: " << iterations_symm << endl;
        cout << "BLAS (dsymm) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0 && duration_blas.count() > 0) {
            double speedup = (double)duration_naive.count() / duration_blas.count();
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;

            double ops = 2.0 * n_symm * n_symm * n_symm * iterations_symm;
            double gflops_blas = ops / (duration_blas.count() * 1e-3) / 1e9;
            double gflops_naive = ops / (duration_naive.count() * 1e-3) / 1e9;
            cout << "BLAS性能: " << fixed << setprecision(2) << gflops_blas << " GFLOPS" << endl;
            cout << "常规实现性能: " << fixed << setprecision(2) << gflops_naive << " GFLOPS" << endl;
        }
        cout << "BLAS优势：利用对称性、分块算法、多线程并行" << endl << endl;
    }

    // 9. Level 3: 对称rank-k更新 dsyrk
    cout << "\n--- 9. 对称rank-k更新 (dsyrk) 性能对比 ---" << endl;
    {
        const int n_syrk = 1024, k_syrk = 512;  // A是1024×512，结果C是1024×1024对称矩阵
        const int iterations_syrk = 3;
        vector<double> A_syrk(n_syrk * k_syrk);
        vector<double> C_syrk_blas(n_syrk * n_syrk);
        vector<double> C_syrk_naive(n_syrk * n_syrk);

        // 初始化矩阵A
        for (int i = 0; i < n_syrk * k_syrk; ++i) {
            A_syrk[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_syrk; ++iter) {
            for (int i = 0; i < n_syrk * n_syrk; ++i) {
                C_syrk_blas[i] = 0.0;
            }
            cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                        n_syrk, k_syrk, 1.0,
                        A_syrk.data(), k_syrk, 0.0, C_syrk_blas.data(), n_syrk);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现：C = A * A^T
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations_syrk; ++iter) {
            for (int i = 0; i < n_syrk; ++i) {
                for (int j = i; j < n_syrk; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < k_syrk; ++k) {
                        sum += A_syrk[i * k_syrk + k] * A_syrk[j * k_syrk + k];
                    }
                    C_syrk_naive[i * n_syrk + j] = sum;
                    C_syrk_naive[j * n_syrk + i] = sum;  // 对称
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵A大小: " << n_syrk << "x" << k_syrk << ", 结果C: " << n_syrk << "x" << n_syrk
             << " (对称), 迭代次数: " << iterations_syrk << endl;
        cout << "BLAS (dsyrk) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0 && duration_blas.count() > 0) {
            double speedup = (double)duration_naive.count() / duration_blas.count();
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;
        }
        cout << "BLAS优势：专门优化对称矩阵计算、利用对称性减少一半计算" << endl << endl;
    }


    cin.get();
    return 0;
}
