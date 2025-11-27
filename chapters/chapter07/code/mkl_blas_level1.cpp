// Intel MKL BLAS Level 1: 向量运算
// 包含函数：点积、范数、缩放、向量加减、复制、交换、索引查找、旋转等

#include "mkl_blas_common.h"

int main() {
    init_console();
    cout << "=== Intel MKL BLAS Level 1: 向量运算示例 ===" << endl;
    cout << "BLAS Level 1 包含以下函数类别：" << endl;
    cout << "  - 基本运算：点积(?dot, ?sdot, ?dotc, ?dotu)、范数(?nrm2, ?asum, ?cabs1)、缩放(?scal)、向量加减(?axpy)" << endl;
    cout << "  - 工具函数：向量复制(?copy, ?copy_batch, ?copy_batch_strided)、向量交换(?swap)、索引查找(i?amax, i?amin)" << endl;
    cout << "  - 旋转函数：向量旋转(?rot, ?rotg)、改进旋转(?rotm, ?rotmg)" << endl;
    cout << "  - 数据类型：单精度(s)、双精度(d)、复数单精度(c)、复数双精度(z)" << endl << endl;

    mkl_set_num_threads(4);

    // ========== 基本运算 ==========
    cout << "\n=== 基本运算 (Basic Operations) ===" << endl;

    // 1. 向量点积 ddot
    cout << "\n--- 1. 向量点积 (ddot) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y = {2.0, 3.0, 4.0};
        print_vector("向量x", x.data(), 3);
        print_vector("向量y", y.data(), 3);

        double dot_result = cblas_ddot(3, x.data(), 1, y.data(), 1);
        cout << "x · y = " << dot_result << endl;

        // 向量与自身的点积（计算模长平方）
        double norm_sq = cblas_ddot(3, x.data(), 1, x.data(), 1);
        cout << "x · x = ||x||^2 = " << norm_sq << endl << endl;
    }

    // 2. 向量范数 dnrm2
    cout << "\n--- 2. 向量范数 (dnrm2) ---" << endl;
    {
        vector<double> x = {3.0, 4.0};
        print_vector("向量x", x.data(), 2);
        double norm_result = cblas_dnrm2(2, x.data(), 1);
        cout << "||x||_2 = " << norm_result << " (应为5.0)" << endl << endl;
    }

    // 3. 向量缩放 dscal: x = alpha * x
    cout << "\n--- 3. 向量缩放 (dscal) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0};
        double alpha = 2.5;
        print_vector("原始向量x", x.data(), 3);
        cblas_dscal(3, alpha, x.data(), 1);
        string label = "缩放后 " + to_string(alpha) + "*x";
        print_vector(label.c_str(), x.data(), 3);
    }

    // 4. 向量加法 daxpy: y = alpha * x + y
    cout << "\n--- 4. 向量加法 (daxpy) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y = {1.0, 1.0, 1.0};
        double alpha = 2.0;
        print_vector("向量x", x.data(), 3);
        print_vector("向量y (初始)", y.data(), 3);
        cblas_daxpy(3, alpha, x.data(), 1, y.data(), 1);
        string label = to_string(alpha) + "*x + y";
        print_vector(label.c_str(), y.data(), 3);
    }

    // 5. 绝对值求和 dasum
    cout << "\n--- 5. 绝对值求和 (dasum) ---" << endl;
    {
        vector<double> x = {-1.0, 2.0, -3.0, 4.0, -5.0};
        print_vector("向量x", x.data(), 5);
        double asum_result = cblas_dasum(5, x.data(), 1);
        cout << "绝对值求和 = " << asum_result << " (应为15.0)" << endl << endl;
    }

    // ========== 工具函数 ==========
    cout << "\n=== 工具函数 (Utility Functions) ===" << endl;

    // 6. 向量复制 dcopy
    cout << "\n--- 6. 向量复制 (dcopy) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
        vector<double> x_copy(5);
        print_vector("原始向量x", x.data(), 5);
        cblas_dcopy(5, x.data(), 1, x_copy.data(), 1);
        print_vector("复制后的向量x_copy", x_copy.data(), 5);
    }

    // 7. 向量交换 dswap
    cout << "\n--- 7. 向量交换 (dswap) ---" << endl;
    {
        vector<double> x = {1.0, 2.0, 3.0};
        vector<double> y = {4.0, 5.0, 6.0};
        print_vector("交换前 x", x.data(), 3);
        print_vector("交换前 y", y.data(), 3);
        cblas_dswap(3, x.data(), 1, y.data(), 1);
        print_vector("交换后 x", x.data(), 3);
        print_vector("交换后 y", y.data(), 3);
    }

    // 8. 最大绝对值索引 idamax
    cout << "\n--- 8. 最大绝对值索引 (idamax) ---" << endl;
    {
        vector<double> x = {-3.0, 2.0, 5.0, -1.0, 4.0};
        cout << "向量: ";
        for (int i = 0; i < 5; ++i) cout << x[i] << " ";
        cout << endl;
        int max_idx = static_cast<int>(cblas_idamax(5, x.data(), 1));
        cout << "最大绝对值元素的索引: " << max_idx << " (值: " << x[max_idx] << ", 绝对值: " << abs(x[max_idx]) << ")" << endl << endl;
    }

    // 9. 最小绝对值索引 idamin
    cout << "\n--- 9. 最小绝对值索引 (idamin) ---" << endl;
    {
        vector<double> x = {-3.0, 2.0, 5.0, -1.0, 4.0};
        cout << "向量: ";
        for (int i = 0; i < 5; ++i) cout << x[i] << " ";
        cout << endl;
        int min_idx = static_cast<int>(cblas_idamin(5, x.data(), 1));
        cout << "最小绝对值元素的索引: " << min_idx << " (值: " << x[min_idx] << ", 绝对值: " << abs(x[min_idx]) << ")" << endl << endl;
    }

    // ========== 旋转函数 ==========
    cout << "\n=== 旋转函数 (Rotation Functions) ===" << endl;

    // 10. 向量旋转 drot
    cout << "\n--- 10. 向量旋转 (drot) ---" << endl;
    {
        vector<double> x = {1.0, 2.0};
        vector<double> y = {3.0, 4.0};
        double c = 0.6;  // cos(角度)
        double s = 0.8;  // sin(角度)
        print_vector("旋转前 x", x.data(), 2);
        print_vector("旋转前 y", y.data(), 2);
        cblas_drot(2, x.data(), 1, y.data(), 1, c, s);
        print_vector("旋转后 x", x.data(), 2);
        print_vector("旋转后 y", y.data(), 2);
        cout << "旋转参数: c = " << c << ", s = " << s << endl << endl;
    }

    // 11. 生成旋转参数 drotg
    cout << "\n--- 11. 生成旋转参数 (drotg) ---" << endl;
    {
        double a = 3.0;
        double b = 4.0;
        double c, s;
        cout << "输入: a = " << a << ", b = " << b << endl;
        cblas_drotg(&a, &b, &c, &s);
        cout << "输出: c = " << c << ", s = " << s << endl;
        cout << "旋转后的 a = " << a << " (应为5.0，即sqrt(3^2+4^2))" << endl << endl;
    }

    // ========== 不同数据类型的示例 ==========
    cout << "\n=== 不同数据类型的示例 ===" << endl;

    // 单精度示例
    cout << "\n--- 单精度 (float) 示例 ---" << endl;
    {
        vector<float> x_f = {1.0f, 2.0f, 3.0f};
        vector<float> y_f = {2.0f, 3.0f, 4.0f};

        // 点积
        float dot_f = cblas_sdot(3, x_f.data(), 1, y_f.data(), 1);
        cout << "单精度点积: " << dot_f << endl;

        // 范数
        float norm_f = cblas_snrm2(3, x_f.data(), 1);
        cout << "单精度范数: " << norm_f << endl;

        // 缩放
        cblas_sscal(3, 2.5f, x_f.data(), 1);
        cout << "单精度缩放后: ";
        for (int i = 0; i < 3; ++i) cout << x_f[i] << " ";
        cout << endl << endl;
    }

    // 复数示例
    cout << "\n--- 复数双精度 (complex<double>) 示例 ---" << endl;
    {
        vector<MKL_Complex16> x_z(3);
        vector<MKL_Complex16> y_z(3);

        // 初始化复数向量
        for (int i = 0; i < 3; ++i) {
            x_z[i].real = i + 1.0;
            x_z[i].imag = i + 0.5;
            y_z[i].real = i + 2.0;
            y_z[i].imag = i + 1.5;
        }

        // 复数点积 (共轭)
        MKL_Complex16 dot_z;
        cblas_zdotc_sub(3, x_z.data(), 1, y_z.data(), 1, &dot_z);
        cout << "复数点积(共轭): (" << dot_z.real << ", " << dot_z.imag << ")" << endl;

        // 复数点积 (非共轭)
        cblas_zdotu_sub(3, x_z.data(), 1, y_z.data(), 1, &dot_z);
        cout << "复数点积(非共轭): (" << dot_z.real << ", " << dot_z.imag << ")" << endl << endl;
    }

    // ========== 性能对比示例 ==========
    cout << "\n=== 性能对比：BLAS vs 常规实现 ===" << endl;

    // 向量点积性能对比
    cout << "\n--- 向量点积性能对比 ---" << endl;
    {
        const int n = 1024 * 1024;  // 1M元素
        const int iterations = 50;
        vector<double> x(n);
        vector<double> y(n);

        for (int i = 0; i < n; ++i) {
            x[i] = i * 0.001;
            y[i] = (i + 1) * 0.001;
        }

        // BLAS实现
        double result_blas = 0.0;
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            result_blas = cblas_ddot(n, x.data(), 1, y.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        double result_naive = 0.0;
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            result_naive = 0.0;
            for (int i = 0; i < n; ++i) {
                result_naive += x[i] * y[i];
            }
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n << ", 迭代次数: " << iterations << endl;
        cout << "BLAS (ddot) 耗时: " << duration_blas.count() << " 微秒, 结果: " << result_blas << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒, 结果: " << result_naive << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：SIMD向量化、循环展开、内存预取" << endl << endl;
    }

    // 向量范数性能对比
    cout << "\n--- 向量范数性能对比 ---" << endl;
    {
        const int n = 1024 * 1024;
        const int iterations = 50;
        vector<double> x(n);

        for (int i = 0; i < n; ++i) {
            x[i] = i * 0.001;
        }

        // BLAS实现
        double result_blas = 0.0;
        auto start_blas = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            result_blas = cblas_dnrm2(n, x.data(), 1);
        }
        auto end_blas = high_resolution_clock::now();
        auto duration_blas = duration_cast<microseconds>(end_blas - start_blas);

        // 常规实现
        double result_naive = 0.0;
        auto start_naive = high_resolution_clock::now();
        for (int iter = 0; iter < iterations; ++iter) {
            result_naive = 0.0;
            for (int i = 0; i < n; ++i) {
                result_naive += x[i] * x[i];
            }
            result_naive = sqrt(result_naive);
        }
        auto end_naive = high_resolution_clock::now();
        auto duration_naive = duration_cast<microseconds>(end_naive - start_naive);

        cout << "向量大小: " << n << ", 迭代次数: " << iterations << endl;
        cout << "BLAS (dnrm2) 耗时: " << duration_blas.count() << " 微秒, 结果: " << result_blas << endl;
        cout << "常规实现耗时: " << duration_naive.count() << " 微秒, 结果: " << result_naive << endl;
        if (duration_naive.count() > 0) {
            cout << "性能提升: " << fixed << setprecision(2)
                 << (double)duration_naive.count() / duration_blas.count() << "x" << endl;
        }
        cout << "BLAS优势：数值稳定性优化、避免溢出" << endl << endl;
    }

    cout << "\n程序执行完成！" << endl;
    cin.get();
    return 0;
}
