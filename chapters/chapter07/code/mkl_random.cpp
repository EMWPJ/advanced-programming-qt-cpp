#include <iostream>
#include <vector>
#include <chrono>

// Intel MKL 随机数生成头文件
#include <mkl.h>
#include <mkl_vsl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_array(const char* name, const double* arr, int size, int max_print = 10) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << arr[i] << " ";
    }
    if (size > max_print) {
        cout << "... (共 " << size << " 个元素)";
    }
    cout << endl << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 随机数生成示例 ===" << endl;

    const int n = 1000000;  // 随机数数量
    vector<double> r(n);

    // 创建随机数流
    VSLStreamStatePtr stream;
    int errcode;

    // 使用默认生成器（MT19937）创建流
    errcode = vslNewStream(&stream, VSL_BRNG_MT19937, 12345);  // 种子=12345
    if (errcode != VSL_STATUS_OK) {
        cout << "创建随机数流失败" << endl;
        return 1;
    }

    cout << "随机数数量: " << n << endl;
    cout << "生成器: MT19937 (Mersenne Twister)" << endl << endl;

    // 1. 均匀分布随机数
    cout << "=== 1. 均匀分布 [0, 1) ===" << endl;
    auto start = high_resolution_clock::now();

    errcode = vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, n, r.data(), 0.0, 1.0);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    if (errcode != VSL_STATUS_OK) {
        cout << "生成均匀分布随机数失败" << endl;
    } else {
        print_array("均匀分布随机数 (前10个)", r.data(), n, 10);
        cout << "生成耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 2. 正态分布随机数
    cout << "=== 2. 正态分布 (均值=0, 标准差=1) ===" << endl;
    start = high_resolution_clock::now();

    errcode = vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, n, r.data(), 0.0, 1.0);

    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    if (errcode != VSL_STATUS_OK) {
        cout << "生成正态分布随机数失败" << endl;
    } else {
        print_array("正态分布随机数 (前10个)", r.data(), n, 10);
        cout << "生成耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 3. 指数分布随机数
    cout << "=== 3. 指数分布 (参数=1.0) ===" << endl;
    start = high_resolution_clock::now();

    errcode = vdRngExponential(VSL_RNG_METHOD_EXPONENTIAL_ICDF, stream, n, r.data(), 0.0, 1.0);

    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    if (errcode != VSL_STATUS_OK) {
        cout << "生成指数分布随机数失败" << endl;
    } else {
        print_array("指数分布随机数 (前10个)", r.data(), n, 10);
        cout << "生成耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 4. 泊松分布随机数（整数）
    cout << "=== 4. 泊松分布 (参数=5.0) ===" << endl;
    vector<int> r_int(n);
    start = high_resolution_clock::now();

    errcode = viRngPoisson(VSL_RNG_METHOD_POISSON_PTPE, stream, n, r_int.data(), 5.0);

    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    if (errcode != VSL_STATUS_OK) {
        cout << "生成泊松分布随机数失败" << endl;
    } else {
        cout << "泊松分布随机数 (前10个): ";
        for (int i = 0; i < min(10, n); ++i) {
            cout << r_int[i] << " ";
        }
        cout << "... (共 " << n << " 个元素)" << endl;
        cout << "生成耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 5. 伽马分布随机数
    cout << "=== 5. 伽马分布 (形状=2.0, 尺度=1.0) ===" << endl;
    start = high_resolution_clock::now();

    errcode = vdRngGamma(VSL_RNG_METHOD_GAMMA_GNORM, stream, n, r.data(), 2.0, 0.0, 1.0);

    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    if (errcode != VSL_STATUS_OK) {
        cout << "生成伽马分布随机数失败" << endl;
    } else {
        print_array("伽马分布随机数 (前10个)", r.data(), n, 10);
        cout << "生成耗时: " << duration.count() << " 微秒" << endl << endl;
    }

    // 6. 性能对比
    cout << "=== 6. 性能对比: MKL vs 标准库 ===" << endl;

    const int test_size = 10000000;
    vector<double> test_mkl(test_size);
    vector<double> test_std(test_size);

    // MKL生成
    start = high_resolution_clock::now();
    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, test_size, test_mkl.data(), 0.0, 1.0);
    end = high_resolution_clock::now();
    auto mkl_time = duration_cast<milliseconds>(end - start);

    // 标准库生成（使用MKL的MT19937作为对比）
    // 注意：这里简化对比，实际标准库需要先创建生成器
    start = high_resolution_clock::now();
    for (int i = 0; i < test_size; ++i) {
        test_std[i] = (double)rand() / RAND_MAX;  // 简单对比
    }
    end = high_resolution_clock::now();
    auto std_time = duration_cast<milliseconds>(end - start);

    cout << "向量长度: " << test_size << endl;
    cout << "MKL生成耗时: " << mkl_time.count() << " 毫秒" << endl;
    cout << "标准库生成耗时: " << std_time.count() << " 毫秒" << endl;
    if (mkl_time.count() > 0) {
        cout << "加速比: " << (double)std_time.count() / mkl_time.count() << "x" << endl;
    }

    // 释放随机数流
    vslDeleteStream(&stream);

    cout << "\n=== MKL随机数生成器类型 ===" << endl;
    cout << "VSL_BRNG_MT19937 - Mersenne Twister (推荐)" << endl;
    cout << "VSL_BRNG_MCG31 - 线性同余生成器" << endl;
    cout << "VSL_BRNG_R250 - 延迟斐波那契生成器" << endl;
    cout << "VSL_BRNG_MRG32K3A - 组合多递归生成器" << endl;
    cout << "VSL_BRNG_SOBOL - Sobol准随机序列" << endl;

    cin.get();
    return 0;
}
