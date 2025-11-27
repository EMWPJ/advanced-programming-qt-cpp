// Intel MKL BLAS Level 3: 矩阵-矩阵运算
// 包含函数：矩阵乘法、对称矩阵乘法、三角矩阵乘法、rank更新、复数厄米矩阵等

#include "mkl_blas_common.h"

int main() {
    init_console();
    cout << "=== Intel MKL BLAS Level 3: 矩阵-矩阵运算示例 ===" << endl;
    cout << "BLAS Level 3 包含以下函数类别：" << endl;
    cout << "  - 基本矩阵-矩阵运算：?gemm, ?symm, ?trmm, ?trsm" << endl;
    cout << "  - Rank更新运算：?syrk, ?syr2k" << endl;
    cout << "  - 复数厄米矩阵：?hemm, ?herk, ?her2k" << endl;
    cout << "  - 数据类型：单精度(s)、双精度(d)、复数单精度(c)、复数双精度(z)" << endl;
    cout << "  - 性能优化：SIMD向量化、多线程并行、内存对齐、缓存优化" << endl << endl;

    mkl_set_num_threads(4);

    // ========== 基本矩阵-矩阵运算 ==========
    cout << "\n=== 基本矩阵-矩阵运算 (Basic Matrix-Matrix) ===" << endl;

    // 1. 矩阵乘法 dgemm: C = alpha * A * B + beta * C
    cout << "\n--- 1. 矩阵乘法 (dgemm) ---" << endl;
    {
        const int m = 3, n = 3, k = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            4.0, 5.0, 6.0,
                            7.0, 8.0, 9.0};
        vector<double> B = {2.0, 0.0, 1.0,
                            1.0, 1.0, 0.0,
                            0.0, 1.0, 2.0};
        vector<double> C(m * n, 0.0);

        print_matrix("矩阵A", A.data(), m, k);
        print_matrix("矩阵B", B.data(), k, n);

        auto start = high_resolution_clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m, n, k, 1.0,
                    A.data(), k, B.data(), n,
                    0.0, C.data(), n);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        print_matrix("A * B", C.data(), m, n);
        cout << "矩阵乘法耗时: " << duration.count() << " 微秒" << endl;
    }

    // 2. 对称矩阵乘法 dsymm
    cout << "\n--- 2. 对称矩阵乘法 (dsymm) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            2.0, 4.0, 5.0,
                            3.0, 5.0, 6.0};  // 对称矩阵
        vector<double> B = {1.0, 0.0, 1.0,
                            0.0, 1.0, 0.0,
                            1.0, 0.0, 1.0};
        vector<double> C(n * n, 0.0);

        print_matrix("对称矩阵A", A.data(), n, n);
        print_matrix("矩阵B", B.data(), n, n);

        cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper,
                    n, n, 1.0,
                    A.data(), n, B.data(), n,
                    0.0, C.data(), n);

        print_matrix("A * B", C.data(), n, n);
    }

    // 3. 三角矩阵乘法 dtrmm
    cout << "\n--- 3. 三角矩阵乘法 (dtrmm) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {1.0, 2.0, 3.0,
                            0.0, 4.0, 5.0,
                            0.0, 0.0, 6.0};  // 上三角矩阵
        vector<double> B = {1.0, 0.0, 1.0,
                            0.0, 1.0, 0.0,
                            1.0, 0.0, 1.0};
        vector<double> C = B;  // 结果会覆盖B

        print_matrix("上三角矩阵A", A.data(), n, n);
        print_matrix("矩阵B (原始)", B.data(), n, n);

        cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper,
                    CblasNoTrans, CblasNonUnit,
                    n, n, 1.0,
                    A.data(), n, C.data(), n);

        print_matrix("A * B", C.data(), n, n);
    }

    // 4. 三角矩阵求解 dtrsm: 解 A*X = alpha*B 或 X*A = alpha*B
    cout << "\n--- 4. 三角矩阵求解 (dtrsm) ---" << endl;
    {
        const int n = 3;
        vector<double> A = {2.0, 1.0, 3.0,
                            0.0, 3.0, 2.0,
                            0.0, 0.0, 4.0};  // 上三角矩阵
        vector<double> B = {4.0, 2.0, 1.0,
                            6.0, 3.0, 2.0,
                            8.0, 4.0, 3.0};  // 右端矩阵
        vector<double> X = B;  // 结果会覆盖B

        print_matrix("上三角矩阵A", A.data(), n, n);
        print_matrix("右端矩阵B", B.data(), n, n);

        cblas_dtrsm(CblasRowMajor, CblasLeft, CblasUpper,
                    CblasNoTrans, CblasNonUnit,
                    n, n, 1.0, A.data(), n, X.data(), n);

        print_matrix("解矩阵 X = A\\B", X.data(), n, n);
        cout << "注意：dtrsm用于求解多个线性方程组" << endl << endl;
    }

    // ========== Rank更新运算 ==========
    cout << "\n=== Rank更新运算 (Rank Updates) ===" << endl;

    // 5. 对称rank-k更新 dsyrk: C = alpha * A * A^T + beta * C
    cout << "\n--- 5. 对称rank-k更新 (dsyrk) ---" << endl;
    {
        const int n = 3, k = 2;
        vector<double> A = {1.0, 2.0,
                            3.0, 4.0,
                            5.0, 6.0};  // 3x2矩阵
        vector<double> C(n * n, 0.0);  // 3x3对称矩阵

        print_matrix("矩阵A (3x2)", A.data(), n, k);

        cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n, k, 1.0,
                    A.data(), k, 0.0, C.data(), n);

        print_matrix("对称矩阵 C = A * A^T", C.data(), n, n);
    }

    // 6. 对称rank-2k更新 dsyr2k: C = alpha * (A*B^T + B*A^T) + beta * C
    cout << "\n--- 6. 对称rank-2k更新 (dsyr2k) ---" << endl;
    {
        const int n = 3, k = 2;
        vector<double> A = {1.0, 2.0,
                            3.0, 4.0,
                            5.0, 6.0};  // 3x2
        vector<double> B = {2.0, 1.0,
                            1.0, 2.0,
                            3.0, 1.0};  // 3x2
        vector<double> C(n * n, 0.0);

        print_matrix("矩阵A (3x2)", A.data(), n, k);
        print_matrix("矩阵B (3x2)", B.data(), n, k);

        cblas_dsyr2k(CblasRowMajor, CblasUpper, CblasNoTrans,
                     n, k, 1.0,
                     A.data(), k, B.data(), k,
                     0.0, C.data(), n);

        print_matrix("对称矩阵 C = A*B^T + B*A^T", C.data(), n, n);
    }

    // ========== 复数厄米矩阵 ==========
    cout << "\n=== 复数厄米矩阵 (Hermitian Matrix) ===" << endl;

    // 7. 厄米矩阵乘法 zhemm
    cout << "\n--- 7. 厄米矩阵乘法 (zhemm) ---" << endl;
    {
        const int n = 2;
        vector<MKL_Complex16> A(n * n);
        vector<MKL_Complex16> B(n * n);
        vector<MKL_Complex16> C(n * n);

        // 初始化厄米矩阵A（上三角存储）
        A[0].real = 1.0; A[0].imag = 0.0;
        A[1].real = 2.0; A[1].imag = 1.0;
        A[2].real = 0.0; A[2].imag = 0.0;
        A[3].real = 3.0; A[3].imag = 0.0;

        // 初始化矩阵B
        B[0].real = 1.0; B[0].imag = 0.0;
        B[1].real = 0.0; B[1].imag = 1.0;
        B[2].real = 1.0; B[2].imag = 1.0;
        B[3].real = 2.0; B[3].imag = 0.0;

        MKL_Complex16 alpha, beta;
        alpha.real = 1.0; alpha.imag = 0.0;
        beta.real = 0.0; beta.imag = 0.0;

        cblas_zhemm(CblasRowMajor, CblasLeft, CblasUpper,
                    n, n, &alpha,
                    A.data(), n, B.data(), n,
                    &beta, C.data(), n);

        print_complex_matrix("厄米矩阵乘法 C = A*B", C.data(), n, n);
    }

    // 8. 厄米rank-k更新 zherk: C = alpha * A * A^H + beta * C
    cout << "\n--- 8. 厄米rank-k更新 (zherk) ---" << endl;
    {
        const int n = 2, k = 2;
        vector<MKL_Complex16> A(n * k);
        vector<MKL_Complex16> C(n * n);

        A[0].real = 1.0; A[0].imag = 1.0;
        A[1].real = 2.0; A[1].imag = 0.0;
        A[2].real = 3.0; A[2].imag = -1.0;
        A[3].real = 4.0; A[3].imag = 2.0;

        cblas_zherk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n, k, 1.0, A.data(), k,
                    0.0, C.data(), n);

        print_complex_matrix("厄米rank-k更新 C = A*A^H", C.data(), n, n);
    }

    // 9. 厄米rank-2k更新 zher2k: C = alpha * (A*B^H + B*A^H) + beta * C
    cout << "\n--- 9. 厄米rank-2k更新 (zher2k) ---" << endl;
    {
        const int n = 2, k = 2;
        vector<MKL_Complex16> A(n * k);
        vector<MKL_Complex16> B(n * k);
        vector<MKL_Complex16> C(n * n);

        A[0].real = 1.0; A[0].imag = 1.0;
        A[1].real = 2.0; A[1].imag = 0.0;
        A[2].real = 3.0; A[2].imag = -1.0;
        A[3].real = 4.0; A[3].imag = 2.0;

        B[0].real = 2.0; B[0].imag = 1.0;
        B[1].real = 1.0; B[1].imag = 0.0;
        B[2].real = 1.0; B[2].imag = 1.0;
        B[3].real = 3.0; B[3].imag = 1.0;

        MKL_Complex16 alpha;
        alpha.real = 1.0; alpha.imag = 0.0;

        cblas_zher2k(CblasRowMajor, CblasUpper, CblasNoTrans,
                     n, k, &alpha,
                     A.data(), k, B.data(), k,
                     0.0, C.data(), n);

        print_complex_matrix("厄米rank-2k更新 C = A*B^H + B*A^H", C.data(), n, n);
    }

    // ========== 不同数据类型的示例 ==========
    cout << "\n=== 不同数据类型的示例 ===" << endl;

    // 单精度示例
    cout << "\n--- 单精度 (float) 矩阵乘法 (sgemm) ---" << endl;
    {
        const int m = 2, n = 2, k = 2;
        vector<float> A = {1.0f, 2.0f, 3.0f, 4.0f};
        vector<float> B = {2.0f, 0.0f, 1.0f, 2.0f};
        vector<float> C(m * n, 0.0f);

        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m, n, k, 1.0f,
                    A.data(), k, B.data(), n,
                    0.0f, C.data(), n);

        print_matrix("单精度矩阵乘法结果", C.data(), m, n);
    }

    // 复数单精度示例
    cout << "\n--- 复数单精度 (complex<float>) 矩阵乘法 (cgemm) ---" << endl;
    {
        const int m = 2, n = 2, k = 2;
        vector<MKL_Complex8> A(m * k);
        vector<MKL_Complex8> B(k * n);
        vector<MKL_Complex8> C(m * n);

        // 初始化复数矩阵
        A[0].real = 1.0f; A[0].imag = 1.0f;
        A[1].real = 2.0f; A[1].imag = 0.0f;
        A[2].real = 3.0f; A[2].imag = -1.0f;
        A[3].real = 4.0f; A[3].imag = 2.0f;

        B[0].real = 1.0f; B[0].imag = 0.0f;
        B[1].real = 0.0f; B[1].imag = 1.0f;
        B[2].real = 1.0f; B[2].imag = 1.0f;
        B[3].real = 2.0f; B[3].imag = 0.0f;

        MKL_Complex8 alpha, beta;
        alpha.real = 1.0f; alpha.imag = 0.0f;
        beta.real = 0.0f; beta.imag = 0.0f;

        cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m, n, k, &alpha,
                    A.data(), k, B.data(), n,
                    &beta, C.data(), n);

        print_complex_matrix("复数单精度矩阵乘法结果", C.data(), m, n);
    }

    // ========== 性能优化示例 ==========
    cout << "\n=== 性能优化示例 ===" << endl;

    // 多线程性能
    cout << "\n--- 多线程性能 ---" << endl;
    {
        const int n_threads = mkl_get_max_threads();
        cout << "当前MKL最大线程数: " << n_threads << endl;
        cout << "MKL自动利用多线程加速矩阵运算" << endl;
        cout << "可以通过 mkl_set_num_threads() 设置线程数" << endl << endl;

        // 测试大规模矩阵乘法
        const int n = 500;
        vector<double> A(n * n);
        vector<double> B(n * n);
        vector<double> C(n * n, 0.0);

        // 初始化
        for (int i = 0; i < n * n; ++i) {
            A[i] = i * 0.001;
            B[i] = i * 0.001;
        }

        auto start = high_resolution_clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    n, n, n, 1.0,
                    A.data(), n, B.data(), n,
                    0.0, C.data(), n);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        cout << "大规模矩阵乘法 (" << n << "x" << n << ") 耗时: "
             << duration.count() << " 毫秒" << endl;
        cout << "MKL自动利用多核CPU和SIMD指令优化性能" << endl << endl;
    }

    // 性能对比：syrk vs gemm
    cout << "\n--- 性能对比：syrk vs gemm ---" << endl;
    {
        const int n = 3, k = 2;
        vector<double> A = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};  // 3x2矩阵
        vector<double> C_syrk(n * n, 0.0);
        vector<double> C_gemm(n * n, 0.0);

        print_matrix("矩阵A (3x2)", A.data(), n, k);

        // 使用dsyrk
        auto start_syrk = high_resolution_clock::now();
        cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                    n, k, 1.0,
                    A.data(), k, 0.0, C_syrk.data(), n);
        auto end_syrk = high_resolution_clock::now();
        auto duration_syrk = duration_cast<nanoseconds>(end_syrk - start_syrk);

        // 使用dgemm计算相同结果
        auto start_gemm = high_resolution_clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    n, n, k, 1.0,
                    A.data(), k, A.data(), k,
                    0.0, C_gemm.data(), n);
        auto end_gemm = high_resolution_clock::now();
        auto duration_gemm = duration_cast<nanoseconds>(end_gemm - start_gemm);

        cout << "dgemm耗时: " << duration_gemm.count() << " 纳秒" << endl;
        cout << "dsyrk耗时: " << duration_syrk.count() << " 纳秒" << endl;
        if (duration_gemm.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_gemm.count() / duration_syrk.count() << "x" << endl;
        }
        cout << "注意：dsyrk专门用于对称矩阵，比使用dgemm更高效" << endl << endl;
    }

    // 缓存优化
    cout << "\n--- 缓存优化 ---" << endl;
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
            vector<double> A(s * s);
            vector<double> B(s * s);
            vector<double> C(s * s, 0.0);

            // 初始化
            for (int i = 0; i < s * s; ++i) {
                A[i] = i * 0.001;
                B[i] = i * 0.001;
            }

            auto start = high_resolution_clock::now();
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                        s, s, s, 1.0,
                        A.data(), s, B.data(), s,
                        0.0, C.data(), s);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);

            if (duration.count() > 0) {
                double gflops = (2.0 * s * s * s) / (duration.count() * 1e-3) / 1e9;
                cout << "  " << s << "x" << s << ": " << duration.count()
                     << " 毫秒 (约 " << fixed << setprecision(2) << gflops << " GFLOPS)" << endl;
            } else {
                cout << "  " << s << "x" << s << ": " << duration.count() << " 毫秒" << endl;
            }
        }
        cout << "注意：MKL自动选择最优的分块大小以适应缓存" << endl << endl;
    }

    // ========== 性能对比示例 ==========
    cout << "\n=== 性能对比：BLAS vs 常规实现 ===" << endl;

    // 矩阵-矩阵乘法性能对比
    cout << "\n--- 矩阵-矩阵乘法性能对比 ---" << endl;
    {
        const int sizes[] = {256, 512, 1024};
        const int iterations = 3;

        for (int size_idx = 0; size_idx < 3; ++size_idx) {
            const int n = sizes[size_idx];
            vector<double> A(n * n);
            vector<double> B(n * n);
            vector<double> C_blas(n * n);
            vector<double> C_naive(n * n);

            // 初始化矩阵
            for (int i = 0; i < n * n; ++i) {
                A[i] = i * 0.001;
                B[i] = (i + 1) * 0.001;
            }

            // BLAS实现
            auto start_blas = high_resolution_clock::now();
            for (int iter = 0; iter < iterations; ++iter) {
                cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                            n, n, n, 1.0,
                            A.data(), n, B.data(), n,
                            0.0, C_blas.data(), n);
            }
            auto end_blas = high_resolution_clock::now();
            auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

            // 常规实现（三重循环）
            auto start_naive = high_resolution_clock::now();
            for (int iter = 0; iter < iterations; ++iter) {
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        C_naive[i * n + j] = 0.0;
                        for (int k = 0; k < n; ++k) {
                            C_naive[i * n + j] += A[i * n + k] * B[k * n + j];
                        }
                    }
                }
            }
            auto end_naive = high_resolution_clock::now();
            auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

            cout << "矩阵大小: " << n << "x" << n << ", 迭代次数: " << iterations << endl;
            cout << "BLAS (dgemm) 耗时: " << duration_blas.count() << " 毫秒" << endl;
            cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
            if (duration_naive.count() > 0 && duration_blas.count() > 0) {
                double speedup = (double)duration_naive.count() / duration_blas.count();
                cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;

                // 计算GFLOPS
                double ops = 2.0 * n * n * n * iterations;
                double gflops_blas = ops / (duration_blas.count() * 1e-3) / 1e9;
                double gflops_naive = ops / (duration_naive.count() * 1e-3) / 1e9;
                cout << "BLAS性能: " << fixed << setprecision(2) << gflops_blas << " GFLOPS" << endl;
                cout << "常规实现性能: " << fixed << setprecision(2) << gflops_naive << " GFLOPS" << endl;
            }
            cout << "BLAS优势：分块算法、多线程并行、SIMD、缓存优化" << endl << endl;
        }
    }

    // 对称矩阵乘法性能对比
    cout << "\n--- 对称矩阵乘法性能对比 ---" << endl;
    {
        const int n = 1024;
        const int iterations = 3;
        vector<double> A(n * n);
        vector<double> B(n * n);
        vector<double> C_blas(n * n);
        vector<double> C_naive(n * n);

        // 初始化对称矩阵A
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                double val = (i * n + j) * 0.001;
                A[i * n + j] = val;
                A[j * n + i] = val;
            }
        }
        // 初始化矩阵B
        for (int i = 0; i < n * n; ++i) {
            B[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper,
                        n, n, 1.0,
                        A.data(), n, B.data(), n,
                        0.0, C_blas.data(), n);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    C_naive[i * n + j] = 0.0;
                    for (int k = 0; k < n; ++k) {
                        C_naive[i * n + j] += A[i * n + k] * B[k * n + j];
                    }
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵大小: " << n << "x" << n << " (A对称), 迭代次数: " << iterations << endl;
        cout << "BLAS (dsymm) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0 && duration_blas.count() > 0) {
            double speedup = (double)duration_naive.count() / duration_blas.count();
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;

            double ops = 2.0 * n * n * n * iterations;
            double gflops_blas = ops / (duration_blas.count() * 1e-3) / 1e9;
            double gflops_naive = ops / (duration_naive.count() * 1e-3) / 1e9;
            cout << "BLAS性能: " << fixed << setprecision(2) << gflops_blas << " GFLOPS" << endl;
            cout << "常规实现性能: " << fixed << setprecision(2) << gflops_naive << " GFLOPS" << endl;
        }
        cout << "BLAS优势：利用对称性、分块算法、多线程并行" << endl << endl;
    }

    // 对称rank-k更新性能对比
    cout << "\n--- 对称rank-k更新性能对比 ---" << endl;
    {
        const int n = 1024, k = 512;  // A是1024×512，结果C是1024×1024对称矩阵
        const int iterations = 3;
        vector<double> A(n * k);
        vector<double> C_blas(n * n);
        vector<double> C_naive(n * n);

        // 初始化矩阵A
        for (int i = 0; i < n * k; ++i) {
            A[i] = i * 0.001;
        }

        // BLAS实现
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < n * n; ++i) {
                C_blas[i] = 0.0;
            }
            cblas_dsyrk(CblasRowMajor, CblasUpper, CblasNoTrans,
                        n, k, 1.0,
                        A.data(), k, 0.0, C_blas.data(), n);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<milliseconds>(end_blas - start_blas);

        // 常规实现：C = A * A^T
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < n; ++i) {
                for (int j = i; j < n; ++j) {
                    double sum = 0.0;
                    for (int k_idx = 0; k_idx < k; ++k_idx) {
                        sum += A[i * k + k_idx] * A[j * k + k_idx];
                    }
                    C_naive[i * n + j] = sum;
                    C_naive[j * n + i] = sum;  // 对称
                }
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<milliseconds>(end_naive - start_naive);

        cout << "矩阵A大小: " << n << "x" << k << ", 结果C: " << n << "x" << n
             << " (对称), 迭代次数: " << iterations << endl;
        cout << "BLAS (dsyrk) 耗时: " << duration_blas.count() << " 毫秒" << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 毫秒" << endl;
        if (duration_naive.count() > 0 && duration_blas.count() > 0) {
            double speedup = (double)duration_naive.count() / duration_blas.count();
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;
        }
        cout << "BLAS优势：专门优化对称矩阵计算、利用对称性减少一半计算" << endl << endl;
    }

    cout << "\n程序执行完成！" << endl;
    cin.get();
    return 0;
}
