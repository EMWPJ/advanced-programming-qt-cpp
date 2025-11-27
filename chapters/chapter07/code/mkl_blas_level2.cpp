// Intel MKL BLAS Level 2: 矩阵-向量运算
// 包含函数：矩阵-向量乘法、对称矩阵-向量、三角矩阵-向量、rank更新、带状矩阵、打包矩阵、复数厄米矩阵等

#include "mkl_blas_common.h"

int main() {
    init_console();
    cout << "=== Intel MKL BLAS Level 2: 矩阵-向量运算示例 ===" << endl;
    cout << "BLAS Level 2 包含以下函数类别：" << endl;
    cout << "  - 基本矩阵-向量运算：?gemv, ?symv, ?trmv, ?trsv" << endl;
    cout << "  - Rank更新运算：?ger, ?gerc, ?geru, ?syr, ?syr2" << endl;
    cout << "  - 带状矩阵：?gbmv, ?sbmv, ?tbmv, ?tbsv" << endl;
    cout << "  - 打包矩阵：?spmv, ?spr, ?spr2, ?tpmv, ?tpsv" << endl;
    cout << "  - 复数厄米矩阵：?hemv, ?her, ?her2, ?hbmv, ?hpmv, ?hpr, ?hpr2" << endl;
    cout << "  - 数据类型：单精度(s)、双精度(d)、复数单精度(c)、复数双精度(z)" << endl << endl;

    mkl_set_num_threads(4);

    // ========== 基本矩阵-向量运算 ==========
    cout << "\n=== 基本矩阵-向量运算 (Basic Matrix-Vector) ===" << endl;

    // 1. 矩阵-向量乘法 dgemv
    cout << "\n--- 1. 矩阵-向量乘法 (dgemv) ---" << endl;
    {
        const int m = 3, n = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            4.0, 5.0, 6.0,
                            7.0, 8.0, 9.0};
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y(m, 0.0);

        print_matrix("矩阵A", A.data(), m, n);
        print_vector("向量x", x.data(), n);

        cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, 1.0,
                    A.data(), n, x.data(), 1, 0.0, y.data(), 1);

        print_vector("A * x", y.data(), m);
    }

    // 2. 对称矩阵-向量乘法 dsymv
    cout << "\n--- 2. 对称矩阵-向量乘法 (dsymv) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            2.0, 4.0, 5.0,
                            3.0, 5.0, 6.0};  // 对称矩阵
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y(n, 0.0);

        print_matrix("对称矩阵A", A.data(), n, n);
        print_vector("向量x", x.data(), n);

        cblas_dsymv(CblasRowMajor, CblasUpper, n, 1.0,
                    A.data(), n, x.data(), 1, 0.0, y.data(), 1);

        print_vector("A * x", y.data(), n);
    }

    // 3. 三角矩阵-向量乘法 dtrmv
    cout << "\n--- 3. 三角矩阵-向量乘法 (dtrmv) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            0.0, 4.0, 5.0,
                            0.0, 0.0, 6.0};  // 上三角矩阵
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> x_result = x;

        print_matrix("上三角矩阵A", A.data(), n, n);
        print_vector("向量x (原始)", x.data(), n);

        cblas_dtrmv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit,
                    n, A.data(), n, x_result.data(), 1);

        print_vector("A * x", x_result.data(), n);
    }

    // 4. 三角矩阵求解 dtrsv: 解 A*x = b
    cout << "\n--- 4. 三角矩阵求解 (dtrsv) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {2.0, 1.0, 3.0,
                            0.0, 3.0, 2.0,
                            0.0, 0.0, 4.0};  // 上三角矩阵
        vector<double> b = {8.0, 11.0, 12.0};
        vector<double> x = b;

        print_matrix("上三角矩阵A", A.data(), n, n);
        print_vector("右端向量b", b.data(), n);

        cblas_dtrsv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit,
                    n, A.data(), n, x.data(), 1);

        print_vector("解向量 x = A\\b", x.data(), n);
    }

    // ========== Rank更新运算 ==========
    cout << "\n=== Rank更新运算 (Rank Updates) ===" << endl;

    // 5. 矩阵-向量外积 dger: A = alpha * x * y^T + A
    cout << "\n--- 5. 矩阵-向量外积 (dger) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y = {2.0, 3.0};
        vector<double> A(3 * 2, 0.0);

        print_vector("向量x", x.data(), 3);
        print_vector("向量y", y.data(), 2);

        cblas_dger(CblasRowMajor, 3, 2, 1.0,
                   x.data(), 1, y.data(), 1, A.data(), 2);

        print_matrix("外积 A = x * y^T", A.data(), 3, 2);
    }

    // 6. 对称rank-1更新 dsyr: A = alpha * x * x^T + A
    cout << "\n--- 6. 对称rank-1更新 (dsyr) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0};  // 单位矩阵（对称）
        vector<double> x = {1.0, 2.0, 3.0};

        print_matrix("原始对称矩阵A", A.data(), n, n);
        print_vector("向量x", x.data(), n);

        cblas_dsyr(CblasRowMajor, CblasUpper, n, 1.0,
                   x.data(), 1, A.data(), n);

        print_matrix("更新后 A = A + x*x^T", A.data(), n, n);
    }

    // 7. 对称rank-2更新 dsyr2: A = alpha * (x*y^T + y*x^T) + A
    cout << "\n--- 7. 对称rank-2更新 (dsyr2) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0};
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y = {2.0, 1.0, 1.0};

        print_matrix("原始对称矩阵A", A.data(), n, n);
        print_vector("向量x", x.data(), n);
        print_vector("向量y", y.data(), n);

        cblas_dsyr2(CblasRowMajor, CblasUpper, n, 1.0,
                    x.data(), 1, y.data(), 1, A.data(), n);

        print_matrix("更新后 A = A + x*y^T + y*x^T", A.data(), n, n);
    }

    // ========== 带状矩阵 ==========
    cout << "\n=== 带状矩阵 (Banded Matrix) ===" << endl;

    // 8. 对称带状矩阵-向量乘法 dsbmv
    cout << "\n--- 8. 对称带状矩阵-向量乘法 (dsbmv) ---" << endl;
    {
        const int n = 4, ku = 1;  // 带宽
        vector<double> A = {0.0, 1.0, 2.0, 3.0,
                            1.0, 2.0, 3.0, 4.0,
                            2.0, 3.0, 4.0, 5.0};  // 对称带状矩阵（打包格式）
        vector<double> x = {1.0, 2.0, 3.0, 4.0};
        vector<double> y(n, 0.0);

        print_vector("向量x", x.data(), n);

        cblas_dsbmv(CblasRowMajor, CblasUpper, n, ku,
                    1.0, A.data(), ku + 1, x.data(), 1,
                    0.0, y.data(), 1);

        print_vector("对称带状矩阵-向量乘法结果", y.data(), n);
        cout << "带状矩阵存储格式：只存储非零带，节省内存" << endl << endl;
    }

    // ========== 打包矩阵 ==========
    cout << "\n=== 打包矩阵 (Packed Matrix) ===" << endl;

    // 9. 对称打包矩阵-向量乘法 dspmv
    cout << "\n--- 9. 对称打包矩阵-向量乘法 (dspmv) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // 打包格式（上三角）
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y(n, 0.0);

        print_vector("向量x", x.data(), n);
        cout << "打包矩阵A（上三角）: ";
        for (double val : A) cout << val << " ";
        cout << endl << endl;

        cblas_dspmv(CblasRowMajor, CblasUpper, n,
                    1.0, A.data(), x.data(), 1,
                    0.0, y.data(), 1);

        print_vector("对称打包矩阵-向量乘法结果", y.data(), n);
        cout << "打包格式：只存储上三角或下三角，节省一半内存" << endl << endl;
    }

    // ========== 复数厄米矩阵 ==========
    cout << "\n=== 复数厄米矩阵 (Hermitian Matrix) ===" << endl;

    // 10. 厄米矩阵-向量乘法 zhemv
    cout << "\n--- 10. 厄米矩阵-向量乘法 (zhemv) ---" << endl;
    {
        const int n = 2;
        vector<MKL_Complex16> A(n * n);
        vector<MKL_Complex16> x(n);
        vector<MKL_Complex16> y(n);

        // 初始化厄米矩阵（上三角存储）
        A[0].real = 1.0; A[0].imag = 0.0;
        A[1].real = 2.0; A[1].imag = 1.0;
        A[2].real = 0.0; A[2].imag = 0.0;  // 下三角不存储
        A[3].real = 3.0; A[3].imag = 0.0;

        x[0].real = 1.0; x[0].imag = 0.0;
        x[1].real = 2.0; x[1].imag = 1.0;

        MKL_Complex16 alpha, beta;
        alpha.real = 1.0; alpha.imag = 0.0;
        beta.real = 0.0; beta.imag = 0.0;

        cout << "厄米矩阵A（上三角）:" << endl;
        cout << "  (" << A[0].real << ", " << A[0].imag << ")\t(" << A[1].real << ", " << A[1].imag << ")" << endl;
        cout << "  (共轭)\t\t(" << A[3].real << ", " << A[3].imag << ")" << endl << endl;

        cblas_zhemv(CblasRowMajor, CblasUpper, n,
                    &alpha, A.data(), n,
                    x.data(), 1, &beta, y.data(), 1);

        cout << "厄米矩阵-向量乘法结果:" << endl;
        for (int i = 0; i < n; ++i) {
            cout << "(" << y[i].real << ", " << y[i].imag << ") ";
        }
        cout << endl << endl;
        cout << "厄米矩阵：A = A^H (共轭转置等于自身)" << endl << endl;
    }

    // 11. 厄米rank-1更新 zher
    cout << "\n--- 11. 厄米rank-1更新 (zher) ---" << endl;
    {
        const int n = 2;
        vector<MKL_Complex16> A(n * n);
        vector<MKL_Complex16> x(n);

        // 初始化单位矩阵（厄米）
        for (int i = 0; i < n * n; ++i) {
            A[i].real = (i % (n + 1) == 0) ? 1.0 : 0.0;
            A[i].imag = 0.0;
        }

        x[0].real = 1.0; x[0].imag = 1.0;
        x[1].real = 2.0; x[1].imag = 0.0;

        cblas_zher(CblasRowMajor, CblasUpper, n, 1.0,
                   x.data(), 1, A.data(), n);

        print_complex_matrix("厄米rank-1更新 A = A + x*x^H", A.data(), n, n);
    }

    // ========== 性能对比示例 ==========
    cout << "\n=== 性能对比：BLAS vs 常规实现 ===" << endl;

    // 矩阵-向量乘法性能对比
    cout << "\n--- 矩阵-向量乘法性能对比 ---" << endl;
    {
        const int m = 1024, n = 1024;
        const int iterations = 10;
        vector<double> A(m * n);
        vector<double> x(n);
        vector<double> y_blas(m);
        vector<double> y_naive(m);

        for (int i = 0; i < m * n; ++i) {
            A[i] = i * 0.001;
        }
        for (int i = 0; i < n; ++i) {
            x[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < m; ++i) {
                y_blas[i] = 0.0;
            }
            cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, 1.0,
                        A.data(), n, x.data(), 1, 0.0, y_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < m; ++i) {
                y_naive[i] = 0.0;
                for (int j = 0; j < n; ++j) {
                    y_naive[i] += A[i * n + j] * x[j];
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << m << "x" << n << ", 迭代次数: " << iterations << endl;
        cout << "BLAS (dgemv) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：缓存优化、分块算法、SIMD向量化" << endl << endl;
    }

    // 对称矩阵-向量乘法性能对比
    cout << "\n--- 对称矩阵-向量乘法性能对比 ---" << endl;
    {
        const int n = 1024;
        const int iterations = 10;
        vector<double> A(n * n);
        vector<double> x(n);
        vector<double> y_blas(n);
        vector<double> y_naive(n);

        // 初始化对称矩阵（上三角存储）
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                double val = (i * n + j) * 0.001;
                A[i * n + j] = val;
                A[j * n + i] = val;  // 对称
            }
        }
        for (int i = 0; i < n; ++i) {
            x[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            cblas_dsymv(CblasRowMajor, CblasUpper, n, 1.0,
                        A.data(), n, x.data(), 1,
                        0.0, y_blas.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < n; ++i) {
                y_naive[i] = 0.0;
                for (int j = 0; j < n; ++j) {
                    y_naive[i] += A[i * n + j] * x[j];
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << n << "x" << n << " (对称), 迭代次数: " << iterations << endl;
        cout << "BLAS (dsymv) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：利用对称性减少计算量、缓存优化" << endl << endl;
    }

    cout << "\n程序执行完成！" << endl;
    cin.get();
    return 0;
}
