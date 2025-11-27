#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <string>

// Intel MKL FFTW3X兼容接口头文件
#include <fftw/fftw3.h>
#include <fftw/fftw3_mkl.h>
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_complex_array(const char* name, const fftw_complex* arr, int size, int max_print = 8) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << "(" << arr[i][0] << ", " << arr[i][1] << ")";
        if (i < print_size - 1) cout << ", ";
        if ((i + 1) % 4 == 0) cout << endl;
    }
    if (size > max_print) {
        cout << "... (共 " << size << " 个元素)";
    }
    cout << endl << endl;
}

void print_real_array(const char* name, const double* arr, int size, int max_print = 8) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << arr[i];
        if (i < print_size - 1) cout << ", ";
        if ((i + 1) % 8 == 0) cout << endl;
    }
    if (size > max_print) {
        cout << "... (共 " << size << " 个元素)";
    }
    cout << endl << endl;
}

// 计算FFT的浮点运算次数（FLOPs）
// 对于N点复数FFT，大约需要 5N*log2(N) 次浮点运算
// 对于N点实数FFT，大约需要 2.5N*log2(N) 次浮点运算
double calculate_fft_flops(int n, bool is_complex = true) {
    if (n <= 0) return 0.0;
    double log2n = log2(static_cast<double>(n));
    if (is_complex) {
        return 5.0 * n * log2n;  // 复数FFT
    } else {
        return 2.5 * n * log2n;  // 实数FFT
    }
}

// 计算GFLOPS
double calculate_gflops(double flops, double time_seconds) {
    if (time_seconds <= 0.0) return 0.0;
    return flops / (time_seconds * 1e9);  // 转换为GFLOPS
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 设置MKL全局线程数为4
    mkl_set_num_threads(4);

    cout << "=== Intel MKL FFTW3X (FFTW接口兼容) 示例 ===" << endl;
    cout << "MKL全局线程数已设置为: " << mkl_get_max_threads() << endl;
    cout << "FFTW3X模块提供与FFTW3完全兼容的接口，使用FFTW的API但底层调用MKL" << endl;
    cout << "主要特性：" << endl;
    cout << "  1. FFTW API兼容：完全兼容FFTW接口，易于移植，无需修改代码" << endl;
    cout << "  2. FFTW计划：fftw_plan创建，计划优化，执行计划" << endl;
    cout << "  3. FFTW类型：fftw_complex, fftw_real，数据类型兼容" << endl;
    cout << "  4. 性能优势：MKL优化实现，比FFTW更快，Intel处理器优化" << endl;
    cout << "  5. 迁移支持：从FFTW迁移，无缝替换，性能提升" << endl << endl;

    // ========== FFTW API兼容性说明 ==========
    cout << "\n=== FFTW API兼容性 ===" << endl;
    cout << "FFTW3X完全兼容FFTW3的API，包括：" << endl;
    cout << "  - 函数名：fftw_plan_dft_1d, fftw_execute, fftw_destroy_plan等" << endl;
    cout << "  - 数据类型：fftw_complex, fftw_real (double)" << endl;
    cout << "  - 计划标志：FFTW_FORWARD, FFTW_BACKWARD, FFTW_ESTIMATE, FFTW_MEASURE等" << endl;
    cout << "  - 内存管理：fftw_malloc, fftw_free（自动对齐内存）" << endl;
    cout << "使用FFTW3X时，只需包含 <fftw3.h> 和 <fftw3_mkl.h>，代码无需修改" << endl << endl;

    // ========== FFTW类型说明 ==========
    cout << "=== FFTW类型 ===" << endl;
    cout << "FFTW3X支持的数据类型：" << endl;
    cout << "  - fftw_complex: 双精度复数，定义为 double[2]，[0]为实部，[1]为虚部" << endl;
    cout << "  - fftw_real: 双精度实数，定义为 double" << endl;
    cout << "这些类型与FFTW3完全兼容，确保代码的可移植性" << endl << endl;

    // 示例1: 一维复数FFT
    cout << "\n=== 1. 一维复数FFT ===" << endl;
    cout << "展示FFTW计划创建和执行的基本流程" << endl;
    {
        const int n = 8;
        fftw_complex *in, *out;
        fftw_plan plan;

        // 分配内存（FFTW使用特殊的内存对齐）
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

        // 初始化输入数据
        for (int i = 0; i < n; ++i) {
            in[i][0] = cos(2.0 * M_PI * i / n);  // 实部
            in[i][1] = sin(2.0 * M_PI * i / n);  // 虚部
        }

        cout << "输入数据:" << endl;
        print_complex_array("输入", in, n);

        // 创建FFT计划（使用MKL后端）
        // FFTW3X会自动使用MKL作为后端，提供更好的性能
        // FFTW_ESTIMATE: 快速创建计划，适合一次性使用
        // FFTW_MEASURE: 较慢但优化计划，适合多次执行
        plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        if (plan == nullptr) {
            cout << "创建FFT计划失败" << endl;
            fftw_free(in);
            fftw_free(out);
            return 1;
        }

        // 执行FFT
        auto start = high_resolution_clock::now();
        fftw_execute(plan);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        print_complex_array("FFT结果", out, n);
        cout << "计算耗时: " << duration.count() << " 微秒" << endl;

        // 创建逆FFT计划
        fftw_plan plan_backward = fftw_plan_dft_1d(n, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
        if (plan_backward != nullptr) {
            fftw_execute(plan_backward);
            // 归一化（FFTW不自动归一化）
            for (int i = 0; i < n; ++i) {
                in[i][0] /= n;
                in[i][1] /= n;
            }
            print_complex_array("逆FFT结果 (归一化后)", in, n);
            fftw_destroy_plan(plan_backward);
        }

        // 清理
        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    // 示例2: 二维FFT
    cout << "\n=== 2. 二维FFT ===" << endl;
    {
        const int rows = 4, cols = 4;
        fftw_complex *in, *out;
        fftw_plan plan;

        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * rows * cols);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * rows * cols);

        // 初始化2D数据
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int idx = i * cols + j;
                in[idx][0] = sin(2.0 * M_PI * i / rows) * cos(2.0 * M_PI * j / cols);
                in[idx][1] = 0.0;
            }
        }

        cout << "2D输入数据 (前8个):" << endl;
        print_complex_array("输入", in, min(8, rows*cols));

        // 创建2D FFT计划
        plan = fftw_plan_dft_2d(rows, cols, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        if (plan == nullptr) {
            cout << "创建2D FFT计划失败" << endl;
            fftw_free(in);
            fftw_free(out);
            return 1;
        }

        // 执行2D FFT
        fftw_execute(plan);

        print_complex_array("2D FFT结果 (前8个)", out, min(8, rows*cols));

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    // 示例3: 实数FFT (R2C)
    cout << "\n=== 3. 实数FFT (R2C) ===" << endl;
    {
        const int n = 8;
        double *in;
        fftw_complex *out;
        fftw_plan plan;

        in = (double*) fftw_malloc(sizeof(double) * n);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (n/2 + 1));

        // 初始化实数输入
        for (int i = 0; i < n; ++i) {
            in[i] = cos(2.0 * M_PI * i / n);
        }

        print_real_array("实数输入", in, n);

        // 创建实数到复数FFT计划
        plan = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);

        if (plan == nullptr) {
            cout << "创建R2C FFT计划失败" << endl;
            fftw_free(in);
            fftw_free(out);
            return 1;
        }

        // 执行R2C FFT
        fftw_execute(plan);

        cout << "R2C FFT结果 (共" << (n/2 + 1) << "个复数):" << endl;
        print_complex_array("输出", out, n/2 + 1);

        // 创建C2R逆FFT计划
        fftw_plan plan_c2r = fftw_plan_dft_c2r_1d(n, out, in, FFTW_ESTIMATE);
        if (plan_c2r != nullptr) {
            fftw_execute(plan_c2r);
            // 归一化
            for (int i = 0; i < n; ++i) {
                in[i] /= n;
            }
            print_real_array("C2R逆FFT结果 (归一化后)", in, n);
            fftw_destroy_plan(plan_c2r);
        }

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    // ========== FFTW计划优化 ==========
    cout << "\n=== FFTW计划优化 ===" << endl;
    cout << "FFTW计划优化标志：" << endl;
    cout << "  - FFTW_ESTIMATE: 快速创建计划（默认），适合一次性使用" << endl;
    cout << "  - FFTW_MEASURE: 测量并优化计划，适合多次执行，性能更好" << endl;
    cout << "  - FFTW_PATIENT: 更彻底的优化，创建时间更长" << endl;
    cout << "  - FFTW_EXHAUSTIVE: 最彻底的优化，创建时间最长" << endl << endl;

    // 示例4: 计划优化对比
    cout << "--- 4. 计划优化对比 ===" << endl;
    {
        const int n = 1024;
        fftw_complex *in, *out;
        fftw_plan plan;

        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

        // 初始化大数据
        for (int i = 0; i < n; ++i) {
            in[i][0] = cos(2.0 * M_PI * i / n);
            in[i][1] = sin(2.0 * M_PI * i / n);
        }

        // 对比ESTIMATE和MEASURE模式的性能
        cout << "创建ESTIMATE计划..." << endl;
        auto start_plan = high_resolution_clock::now();
        fftw_plan plan_estimate = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        auto end_plan = high_resolution_clock::now();
        auto duration_plan_estimate = duration_cast<microseconds>(end_plan - start_plan);
        cout << "ESTIMATE计划创建耗时: " << duration_plan_estimate.count() << " 微秒" << endl;

        cout << "创建MEASURE计划（优化）..." << endl;
        start_plan = high_resolution_clock::now();
        plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_MEASURE);
        end_plan = high_resolution_clock::now();
        auto duration_plan_measure = duration_cast<microseconds>(end_plan - start_plan);
        cout << "MEASURE计划创建耗时: " << duration_plan_measure.count() << " 微秒" << endl;
        cout << "注意：MEASURE模式创建计划较慢，但执行速度更快" << endl << endl;

        if (plan == nullptr) {
            cout << "创建优化FFT计划失败" << endl;
            fftw_free(in);
            fftw_free(out);
            return 1;
        }

        // 执行多次以测量性能
        const int iterations = 100;

        // 测试ESTIMATE计划性能
        auto start = high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            fftw_execute(plan_estimate);
        }
        auto end = high_resolution_clock::now();
        auto duration_estimate = duration_cast<microseconds>(end - start);

        // 测试MEASURE计划性能
        start = high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            fftw_execute(plan);
        }
        end = high_resolution_clock::now();
        auto duration_measure = duration_cast<microseconds>(end - start);

        cout << "执行 " << iterations << " 次FFT (n=" << n << ") 性能对比:" << endl;
        cout << "ESTIMATE计划: " << duration_estimate.count() << " 微秒 (平均 "
             << duration_estimate.count() / (double)iterations << " 微秒/次)" << endl;
        cout << "MEASURE计划: " << duration_measure.count() << " 微秒 (平均 "
             << duration_measure.count() / (double)iterations << " 微秒/次)" << endl;
        cout << "性能提升: " << (double)duration_estimate.count() / duration_measure.count() << "x" << endl;
        cout << "结论：多次执行时，MEASURE计划更优" << endl << endl;

        fftw_destroy_plan(plan_estimate);
        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    // 示例5: 批量FFT
    cout << "\n=== 5. 批量FFT ===" << endl;
    {
        const int n = 4;  // 每个FFT的长度
        const int batch = 3;  // 批量数量

        fftw_complex *in, *out;
        fftw_plan plan;

        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n * batch);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n * batch);

        // 初始化批量数据
        for (int b = 0; b < batch; ++b) {
            for (int i = 0; i < n; ++i) {
                int idx = b * n + i;
                double phase = 2.0 * M_PI * (i + b) / n;
                in[idx][0] = cos(phase);
                in[idx][1] = sin(phase);
            }
        }

        // 创建批量FFT计划
        // FFTW3支持批量处理，通过设置输入/输出距离
        int rank = 1;
        int n_array[] = {n};
        int howmany = batch;
        int idist = n, odist = n;
        int istride = 1, ostride = 1;
        int *inembed = nullptr, *onembed = nullptr;

        plan = fftw_plan_many_dft(rank, n_array, howmany,
                                  in, inembed, istride, idist,
                                  out, onembed, ostride, odist,
                                  FFTW_FORWARD, FFTW_ESTIMATE);

        if (plan == nullptr) {
            cout << "创建批量FFT计划失败" << endl;
            fftw_free(in);
            fftw_free(out);
            return 1;
        }

        // 执行批量FFT
        fftw_execute(plan);

        cout << "批量FFT结果:" << endl;
        for (int b = 0; b < batch; ++b) {
            cout << "Batch " << b << ": ";
            for (int i = 0; i < n; ++i) {
                int idx = b * n + i;
                cout << "(" << out[idx][0] << "," << out[idx][1] << ") ";
            }
            cout << endl;
        }

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    // ========== 迁移支持示例 ==========
    cout << "\n=== 从FFTW迁移到FFTW3X ===" << endl;
    cout << "迁移步骤：" << endl;
    cout << "  1. 包含头文件：将 #include <fftw3.h> 改为：" << endl;
    cout << "     #include <fftw3.h>" << endl;
    cout << "     #include <fftw3_mkl.h>" << endl << endl;
    cout << "  2. 链接库：链接MKL库而不是FFTW库" << endl;
    cout << "  3. 代码无需修改：所有FFTW函数调用保持不变" << endl << endl;

    cout << "【FFTW代码示例】" << endl;
    cout << "  #include <fftw3.h>" << endl;
    cout << "  fftw_complex *in, *out;" << endl;
    cout << "  fftw_plan plan;" << endl;
    cout << "  plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);" << endl;
    cout << "  fftw_execute(plan);" << endl << endl;

    cout << "【FFTW3X迁移后】" << endl;
    cout << "  #include <fftw3.h>" << endl;
    cout << "  #include <fftw3_mkl.h>  // 添加这一行" << endl;
    cout << "  // 其他代码完全相同，无需修改" << endl;
    cout << "  // 自动使用MKL后端，获得性能提升" << endl << endl;

    // ========== 性能优势展示 ==========
    cout << "\n=== 性能优势 ===" << endl;
    cout << "MKL FFTW3X相比标准FFTW的优势：" << endl;
    cout << "  1. Intel处理器优化：针对Intel CPU架构优化" << endl;
    cout << "  2. SIMD向量化：充分利用AVX/AVX2/AVX-512指令" << endl;
    cout << "  3. 多线程并行：自动利用多核CPU" << endl;
    cout << "  4. 内存对齐优化：fftw_malloc自动对齐，提高缓存效率" << endl;
    cout << "  5. 更好的数值稳定性：MKL优化的算法" << endl << endl;

    // 示例6: 性能测试（包含GFLOPS）
    cout << "--- 6. 性能测试（包含GFLOPS） ---" << endl;
    {
        const int n_perf = 65536 * 64;  // 4194304 = 2^22
        const int iterations_perf = 10;
        fftw_complex *in_perf, *out_perf;
        fftw_plan plan_perf;

        cout << "测试配置：" << endl;
        cout << "  数据长度: " << n_perf << " (2^22)" << endl;
        cout << "  迭代次数: " << iterations_perf << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        in_perf = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_perf);
        out_perf = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n_perf);

        // 初始化
        cout << "初始化数据中..." << flush;
        for (int i = 0; i < n_perf; ++i) {
            double phase = 2.0 * M_PI * i / n_perf;
            in_perf[i][0] = cos(phase);
            in_perf[i][1] = sin(phase);
        }
        cout << " 完成" << endl;

        // 创建优化计划
        cout << "创建FFTW计划（MEASURE模式，优化中）..." << flush;
        plan_perf = fftw_plan_dft_1d(n_perf, in_perf, out_perf, FFTW_FORWARD, FFTW_MEASURE);
        cout << " 完成" << endl;

        if (plan_perf != nullptr) {
            // 预热
            cout << "预热中..." << flush;
            for (int i = 0; i < 3; ++i) {
                fftw_execute(plan_perf);
            }
            cout << " 完成" << endl;

            // 性能测试
            cout << "执行性能测试中..." << flush;
            auto start_perf = high_resolution_clock::now();
            for (int i = 0; i < iterations_perf; ++i) {
                fftw_execute(plan_perf);
            }
            auto end_perf = high_resolution_clock::now();
            cout << " 完成" << endl;

            auto duration_us = duration_cast<microseconds>(end_perf - start_perf);
            auto duration_ms = duration_cast<milliseconds>(end_perf - start_perf);
            double time_seconds = duration_us.count() / 1e6;
            double avg_time_ms = duration_ms.count() / (double)iterations_perf;

            // 计算GFLOPS
            double flops_per_fft = calculate_fft_flops(n_perf, true);
            double total_flops = flops_per_fft * iterations_perf;
            double gflops = calculate_gflops(total_flops, time_seconds);

            cout << "\n性能测试结果：" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << "时间统计:" << endl;
            cout << "  总耗时: " << duration_ms.count() << " 毫秒 (" << duration_us.count() << " 微秒)" << endl;
            cout << "  平均每次: " << fixed << setprecision(3) << avg_time_ms << " 毫秒" << endl;
            cout << "  吞吐量: " << fixed << setprecision(1) << (iterations_perf * 1000.0) / duration_ms.count() << " FFT/秒" << endl;
            cout << endl;
            cout << "性能指标:" << endl;
            cout << "  每次FFT运算量: " << fixed << setprecision(2) << flops_per_fft / 1e6 << " MFLOPs" << endl;
            cout << "  总运算量: " << fixed << setprecision(2) << total_flops / 1e9 << " GFLOPs" << endl;
            cout << "  性能: " << fixed << setprecision(3) << gflops << " GFLOPS" << endl;
            cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
            cout << "MKL FFTW3X充分利用Intel处理器特性，性能优异" << endl << endl;

            fftw_destroy_plan(plan_perf);
        }

        fftw_free(in_perf);
        fftw_free(out_perf);
    }

    // 示例7: 多规模性能对比测试（GFLOPS）
    cout << "\n--- 7. 多规模性能对比测试（GFLOPS） ---" << endl;
    {
        vector<int> test_sizes = {256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304};
        const int iterations = 500;

        cout << "测试不同规模的FFT性能（" << iterations << "次迭代）:" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "规模(N)\t\t时间(ms)\t平均(μs)\t吞吐量(FFT/s)\tGFLOPS" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        for (int n : test_sizes) {
            fftw_complex *in_test, *out_test;
            fftw_plan plan_test;

            in_test = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
            out_test = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

            // 初始化
            for (int i = 0; i < n; ++i) {
                double phase = 2.0 * M_PI * i / n;
                in_test[i][0] = cos(phase);
                in_test[i][1] = sin(phase);
            }

            // 创建计划（使用ESTIMATE以加快计划创建）
            plan_test = fftw_plan_dft_1d(n, in_test, out_test, FFTW_FORWARD, FFTW_ESTIMATE);

            if (plan_test != nullptr) {
                // 预热
                for (int i = 0; i < 3; ++i) {
                    fftw_execute(plan_test);
                }

                // 性能测试
                auto start = high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                    fftw_execute(plan_test);
                }
                auto end = high_resolution_clock::now();

                auto duration_us = duration_cast<microseconds>(end - start);
                auto duration_ms = duration_cast<milliseconds>(end - start);
                double time_seconds = duration_us.count() / 1e6;

                double flops_per_fft = calculate_fft_flops(n, true);
                double total_flops = flops_per_fft * iterations;
                double gflops = calculate_gflops(total_flops, time_seconds);

                cout << n << "\t\t"
                     << duration_ms.count() << "\t\t"
                     << fixed << setprecision(2) << duration_us.count() / (double)iterations << "\t\t"
                     << fixed << setprecision(1) << (iterations * 1000.0) / duration_ms.count() << "\t\t"
                     << fixed << setprecision(3) << gflops << endl;

                fftw_destroy_plan(plan_test);
            }

            fftw_free(in_test);
            fftw_free(out_test);
        }
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "注：GFLOPS值越高表示性能越好，MKL FFTW3X针对不同规模进行了优化" << endl << endl;
    }

    // ========== 数据类型兼容性 ==========
    cout << "\n=== 数据类型兼容性 ===" << endl;
    cout << "FFTW3X完全兼容FFTW的数据类型：" << endl;
    {
        // 演示fftw_complex的使用
        fftw_complex z;
        z[0] = 1.0;  // 实部
        z[1] = 2.0;  // 虚部
        cout << "fftw_complex示例: (" << z[0] << ", " << z[1] << ")" << endl;
        cout << "  - z[0]: 实部 (real part)" << endl;
        cout << "  - z[1]: 虚部 (imaginary part)" << endl << endl;

        // 演示fftw_real的使用（fftw_real实际上就是double类型）
        double r = 3.14159;
        cout << "fftw_real示例: " << r << endl;
        cout << "  - fftw_real就是double类型" << endl << endl;
    }

    // ========== 内存管理 ==========
    cout << "=== 内存管理 ===" << endl;
    cout << "FFTW3X提供特殊的内存管理函数：" << endl;
    cout << "  - fftw_malloc: 分配对齐内存，提高性能" << endl;
    cout << "  - fftw_free: 释放fftw_malloc分配的内存" << endl;
    cout << "  - fftw_cleanup: 释放FFTW内部资源" << endl;
    cout << "注意：使用fftw_malloc分配的内存必须用fftw_free释放" << endl << endl;

    // 清理FFTW（释放内部资源）
    fftw_cleanup();

    cout << "\n=== 总结 ===" << endl;
    cout << "MKL FFTW3X提供了与FFTW3完全兼容的接口，具有以下优势：" << endl;
    cout << "  1. 完全兼容：无需修改代码，只需添加头文件" << endl;
    cout << "  2. 性能提升：MKL优化实现，充分利用Intel处理器特性" << endl;
    cout << "  3. 易于迁移：从FFTW无缝迁移，获得性能提升" << endl;
    cout << "  4. 类型兼容：完全兼容FFTW的数据类型" << endl;
    cout << "  5. 计划优化：支持多种计划优化模式" << endl;

    cin.get();
    return 0;
}
