#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// Intel MKL VML (Vector Math Library) 头文件
#include <mkl_vml.h>
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
    cout << "=== Intel MKL VML (向量数学库) 示例 ===" << endl;
    cout << "VML模块提供高度优化的向量数学函数，支持SIMD向量化和多线程并行" << endl;
    cout << "主要函数类别：" << endl;
    cout << "  1. Arithmetic Functions（算术函数）：Add, Sub, Mul, Div" << endl;
    cout << "  2. Power and Root Functions（幂和根函数）：Pow, Sqrt, Cbrt, Inv, InvSqrt" << endl;
    cout << "  3. Exponential and Logarithmic Functions（指数和对数函数）：Exp, Ln, Log10, Log2, Exp2, Exp10, Log1p, Expm1" << endl;
    cout << "  4. Trigonometric Functions（三角函数）：Sin, Cos, Tan, Asin, Acos, Atan, Atan2, SinCos" << endl;
    cout << "  5. Hyperbolic Functions（双曲函数）：Sinh, Cosh, Tanh, Asinh, Acosh, Atanh" << endl;
    cout << "  6. Special Functions（特殊函数）：Erf, Erfc, Gamma, LGamma, TGamma" << endl;
    cout << "  7. Rounding Functions（舍入函数）：Floor, Ceil, Round, Trunc, NearbyInt" << endl;
    cout << "  8. Miscellaneous Functions（杂项函数）：Abs, Fabs, Fmod, Remainder, CopySign, NextAfter" << endl;
    cout << "  9. 精度模式：高精度模式 (HA)，低精度模式 (LA)，增强精度模式 (EP)" << endl;
    cout << "  10. SIMD优化：自动向量化，SSE/AVX指令集，多线程并行，批量处理优化" << endl << endl;

    const int n = 1000000;  // 向量长度

    // 创建输入向量
    vector<double> x(n);
    vector<double> y(n);
    vector<double> result(n);

    // 初始化向量
    for (int i = 0; i < n; ++i) {
        x[i] = (double)(i + 1) / n * M_PI;  // 0 到 π
        y[i] = (double)(i + 1) / n * 2.0;   // 0 到 2
    }

    cout << "向量长度: " << n << endl;
    print_array("输入向量 x (前10个)", x.data(), n, 10);
    print_array("输入向量 y (前10个)", y.data(), n, 10);

    // 准备数据
    vector<double> y_positive = y;
    for (int i = 0; i < n; ++i) {
        y_positive[i] = abs(y[i]) + 1.0;  // 确保为正数
    }

    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    // ========== 1. Arithmetic Functions（算术函数）==========
    cout << "\n=== 1. Arithmetic Functions（算术函数）===" << endl;
    cout << "提供向量级的加减乘除运算，元素级操作，高度优化" << endl << endl;

    // 1.1 向量加法 vdAdd
    cout << "--- 1.1 向量加法 (vdAdd) ---" << endl;
    start = high_resolution_clock::now();
    vdAdd(n, x.data(), y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("x + y (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 1.2 向量减法 vdSub
    cout << "--- 1.2 向量减法 (vdSub) ---" << endl;
    start = high_resolution_clock::now();
    vdSub(n, x.data(), y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("x - y (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 1.3 向量乘法 vdMul
    cout << "--- 1.3 向量乘法 (vdMul) ---" << endl;
    start = high_resolution_clock::now();
    vdMul(n, x.data(), y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("x * y (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 1.4 向量除法 vdDiv
    cout << "--- 1.4 向量除法 (vdDiv) ---" << endl;
    start = high_resolution_clock::now();
    vdDiv(n, x.data(), y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("x / (y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 2. Power and Root Functions（幂和根函数）==========
    cout << "\n=== 2. Power and Root Functions（幂和根函数）===" << endl;
    cout << "提供幂运算和根运算函数，包括平方根、立方根、倒数等" << endl << endl;

    // 2.1 幂函数 vdPow
    cout << "--- 2.1 幂函数 (vdPow) ---" << endl;
    start = high_resolution_clock::now();
    vdPow(n, y_positive.data(), x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("(y+1)^x (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 2.2 平方根函数 vdSqrt
    cout << "--- 2.2 平方根函数 (vdSqrt) ---" << endl;
    start = high_resolution_clock::now();
    vdSqrt(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("sqrt(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 2.3 立方根函数 vdCbrt
    cout << "--- 2.3 立方根函数 (vdCbrt) ---" << endl;
    start = high_resolution_clock::now();
    vdCbrt(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("cbrt(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 2.4 倒数函数 vdInv
    cout << "--- 2.4 倒数函数 (vdInv) ---" << endl;
    start = high_resolution_clock::now();
    vdInv(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("1/(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 2.5 平方根倒数函数 vdInvSqrt
    cout << "--- 2.5 平方根倒数函数 (vdInvSqrt) ---" << endl;
    start = high_resolution_clock::now();
    vdInvSqrt(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("1/sqrt(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 3. Exponential and Logarithmic Functions（指数和对数函数）==========
    cout << "\n=== 3. Exponential and Logarithmic Functions（指数和对数函数）===" << endl;
    cout << "提供指数函数和对数函数，包括自然对数、常用对数、以2为底的对数等" << endl << endl;

    // 3.1 指数函数 vdExp
    cout << "--- 3.1 指数函数 (vdExp) ---" << endl;
    start = high_resolution_clock::now();
    vdExp(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("exp(y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.2 自然对数函数 vdLn
    cout << "--- 3.2 自然对数函数 (vdLn) ---" << endl;
    start = high_resolution_clock::now();
    vdLn(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("ln(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.3 常用对数函数 vdLog10
    cout << "--- 3.3 常用对数函数 (vdLog10) ---" << endl;
    start = high_resolution_clock::now();
    vdLog10(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("log10(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.4 以2为底的对数函数 vdLog2
    cout << "--- 3.4 以2为底的对数函数 (vdLog2) ---" << endl;
    start = high_resolution_clock::now();
    vdLog2(n, y_positive.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("log2(y+1) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.5 以2为底的指数函数 vdExp2
    cout << "--- 3.5 以2为底的指数函数 (vdExp2) ---" << endl;
    start = high_resolution_clock::now();
    vdExp2(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("2^y (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.6 以10为底的指数函数 vdExp10
    cout << "--- 3.6 以10为底的指数函数 (vdExp10) ---" << endl;
    start = high_resolution_clock::now();
    vdExp10(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("10^y (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.7 log1p函数 vdLog1p (ln(1+x))
    cout << "--- 3.7 log1p函数 (vdLog1p) - ln(1+x) ---" << endl;
    start = high_resolution_clock::now();
    vdLog1p(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("log1p(y) = ln(1+y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 3.8 expm1函数 vdExpm1 (exp(x)-1)
    cout << "--- 3.8 expm1函数 (vdExpm1) - exp(x)-1 ---" << endl;
    start = high_resolution_clock::now();
    vdExpm1(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("expm1(y) = exp(y)-1 (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 4. Trigonometric Functions（三角函数）==========
    cout << "\n=== 4. Trigonometric Functions（三角函数）===" << endl;
    cout << "提供标准三角函数和反三角函数，包括sin/cos/tan及其反函数" << endl << endl;

    // 4.1 正弦函数 vdSin
    cout << "--- 4.1 正弦函数 (vdSin) ---" << endl;
    start = high_resolution_clock::now();
    vdSin(n, x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("sin(x) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.2 余弦函数 vdCos
    cout << "--- 4.2 余弦函数 (vdCos) ---" << endl;
    start = high_resolution_clock::now();
    vdCos(n, x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("cos(x) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.3 正切函数 vdTan
    cout << "--- 4.3 正切函数 (vdTan) ---" << endl;
    start = high_resolution_clock::now();
    vdTan(n, x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("tan(x) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.4 反正弦函数 vdAsin
    cout << "--- 4.4 反正弦函数 (vdAsin) ---" << endl;
    vector<double> x_asin(n);
    for (int i = 0; i < n; ++i) {
        x_asin[i] = sin(x[i]);  // 确保在[-1, 1]范围内
    }
    start = high_resolution_clock::now();
    vdAsin(n, x_asin.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("asin(sin(x)) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.5 反余弦函数 vdAcos
    cout << "--- 4.5 反余弦函数 (vdAcos) ---" << endl;
    start = high_resolution_clock::now();
    vdAcos(n, x_asin.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("acos(sin(x)) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.6 反正切函数 vdAtan
    cout << "--- 4.6 反正切函数 (vdAtan) ---" << endl;
    start = high_resolution_clock::now();
    vdAtan(n, x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("atan(x) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.7 双参数反正切函数 vdAtan2
    cout << "--- 4.7 双参数反正切函数 (vdAtan2) ---" << endl;
    start = high_resolution_clock::now();
    vdAtan2(n, y.data(), x.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("atan2(y, x) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 4.8 同时计算sin和cos vdSinCos
    cout << "--- 4.8 同时计算sin和cos (vdSinCos) ---" << endl;
    vector<double> sin_result(n), cos_result(n);
    start = high_resolution_clock::now();
    vdSinCos(n, x.data(), sin_result.data(), cos_result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("sin(x) (前10个)", sin_result.data(), n, 10);
    print_array("cos(x) (前10个)", cos_result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl;
    cout << "注意：同时计算sin和cos比分别计算更高效" << endl << endl;

    // ========== 5. Hyperbolic Functions（双曲函数）==========
    cout << "\n=== 5. Hyperbolic Functions（双曲函数）===" << endl;
    cout << "提供双曲函数和反双曲函数，包括sinh/cosh/tanh及其反函数" << endl << endl;

    // 5.1 双曲正弦函数 vdSinh
    cout << "--- 5.1 双曲正弦函数 (vdSinh) ---" << endl;
    start = high_resolution_clock::now();
    vdSinh(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("sinh(y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 5.2 双曲余弦函数 vdCosh
    cout << "--- 5.2 双曲余弦函数 (vdCosh) ---" << endl;
    start = high_resolution_clock::now();
    vdCosh(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("cosh(y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 5.3 双曲正切函数 vdTanh
    cout << "--- 5.3 双曲正切函数 (vdTanh) ---" << endl;
    start = high_resolution_clock::now();
    vdTanh(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("tanh(y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 5.4 反双曲正弦函数 vdAsinh
    cout << "--- 5.4 反双曲正弦函数 (vdAsinh) ---" << endl;
    start = high_resolution_clock::now();
    vdAsinh(n, y.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("asinh(y) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 5.5 反双曲余弦函数 vdAcosh
    cout << "--- 5.5 反双曲余弦函数 (vdAcosh) ---" << endl;
    vector<double> y_acosh = y_positive;
    for (int i = 0; i < n; ++i) {
        y_acosh[i] = y_positive[i] + 1.0;  // 确保 > 1
    }
    start = high_resolution_clock::now();
    vdAcosh(n, y_acosh.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("acosh(y+2) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 5.6 反双曲正切函数 vdAtanh
    cout << "--- 5.6 反双曲正切函数 (vdAtanh) ---" << endl;
    vector<double> y_atanh(n);
    for (int i = 0; i < n; ++i) {
        y_atanh[i] = (y[i] - 1.0) / (y[i] + 1.0);  // 确保在(-1, 1)范围内
    }
    start = high_resolution_clock::now();
    vdAtanh(n, y_atanh.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("atanh((y-1)/(y+1)) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 6. Special Functions（特殊函数）==========
    cout << "\n=== 6. Special Functions（特殊函数）===" << endl;
    cout << "提供特殊数学函数，包括误差函数、伽马函数等" << endl << endl;

    // 6.1 误差函数 vdErf
    cout << "--- 6.1 误差函数 (vdErf) ---" << endl;
    vector<double> x_erf(n);
    for (int i = 0; i < n; ++i) {
        x_erf[i] = (double)(i + 1) / n * 3.0 - 1.5;  // -1.5 到 1.5
    }
    start = high_resolution_clock::now();
    vdErf(n, x_erf.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("erf(x_erf) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 6.2 互补误差函数 vdErfc
    cout << "--- 6.2 互补误差函数 (vdErfc) ---" << endl;
    start = high_resolution_clock::now();
    vdErfc(n, x_erf.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("erfc(x_erf) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 6.3 伽马函数 vdTGamma
    cout << "--- 6.3 伽马函数 (vdTGamma) ---" << endl;
    vector<double> x_gamma(n);
    for (int i = 0; i < n; ++i) {
        x_gamma[i] = (double)(i + 1) / n * 5.0 + 0.1;  // 0.1 到 5.1（避免0和负整数）
    }
    start = high_resolution_clock::now();
    vdTGamma(n, x_gamma.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("tgamma(x_gamma) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 6.4 对数伽马函数 vdLGamma
    cout << "--- 6.4 对数伽马函数 (vdLGamma) ---" << endl;
    start = high_resolution_clock::now();
    vdLGamma(n, x_gamma.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("lgamma(x_gamma) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 7. Rounding Functions（舍入函数）==========
    cout << "\n=== 7. Rounding Functions（舍入函数）===" << endl;
    cout << "提供舍入函数，包括向下取整、向上取整、四舍五入等" << endl << endl;

    // 7.1 向下取整函数 vdFloor
    cout << "--- 7.1 向下取整函数 (vdFloor) ---" << endl;
    vector<double> x_round(n);
    for (int i = 0; i < n; ++i) {
        x_round[i] = (double)(i + 1) / n * 10.0 - 5.0;  // -5 到 5
    }
    start = high_resolution_clock::now();
    vdFloor(n, x_round.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("floor(x_round) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 7.2 向上取整函数 vdCeil
    cout << "--- 7.2 向上取整函数 (vdCeil) ---" << endl;
    start = high_resolution_clock::now();
    vdCeil(n, x_round.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("ceil(x_round) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 7.3 四舍五入函数 vdRound
    cout << "--- 7.3 四舍五入函数 (vdRound) ---" << endl;
    start = high_resolution_clock::now();
    vdRound(n, x_round.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("round(x_round) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 7.4 截断函数 vdTrunc
    cout << "--- 7.4 截断函数 (vdTrunc) ---" << endl;
    start = high_resolution_clock::now();
    vdTrunc(n, x_round.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("trunc(x_round) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 7.5 最近整数函数 vdNearbyInt
    cout << "--- 7.5 最近整数函数 (vdNearbyInt) ---" << endl;
    start = high_resolution_clock::now();
    vdNearbyInt(n, x_round.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("nearbyint(x_round) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 8. Miscellaneous Functions（杂项函数）==========
    cout << "\n=== 8. Miscellaneous Functions（杂项函数）===" << endl;
    cout << "提供其他常用数学函数，包括绝对值、取模、符号复制等" << endl << endl;

    // 8.1 绝对值函数 vdAbs
    cout << "--- 8.1 绝对值函数 (vdAbs) ---" << endl;
    vector<double> x_abs(n);
    for (int i = 0; i < n; ++i) {
        x_abs[i] = (double)(i + 1) / n * 10.0 - 5.0;  // -5 到 5
    }
    start = high_resolution_clock::now();
    vdAbs(n, x_abs.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("abs(x_abs) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 8.2 浮点绝对值函数 vdAbs（使用vdAbs，vdFabs在某些MKL版本中可能不可用）
    cout << "--- 8.2 浮点绝对值函数 (vdAbs) ---" << endl;
    start = high_resolution_clock::now();
    vdAbs(n, x_abs.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("fabs(x_abs) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 8.3 浮点取模函数 vdFmod
    cout << "--- 8.3 浮点取模函数 (vdFmod) ---" << endl;
    vector<double> y_mod(n);
    for (int i = 0; i < n; ++i) {
        y_mod[i] = (double)(i + 1) / n * 3.0 + 0.5;  // 0.5 到 3.5（避免0）
    }
    start = high_resolution_clock::now();
    vdFmod(n, x_abs.data(), y_mod.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("fmod(x_abs, y_mod) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 8.4 余数函数 vdRemainder
    cout << "--- 8.4 余数函数 (vdRemainder) ---" << endl;
    start = high_resolution_clock::now();
    vdRemainder(n, x_abs.data(), y_mod.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("remainder(x_abs, y_mod) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 8.5 符号复制函数 vdCopySign
    cout << "--- 8.5 符号复制函数 (vdCopySign) ---" << endl;
    vector<double> y_sign(n);
    for (int i = 0; i < n; ++i) {
        y_sign[i] = (i % 2 == 0) ? 1.0 : -1.0;  // 交替正负号
    }
    start = high_resolution_clock::now();
    vdCopySign(n, x_abs.data(), y_sign.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("copysign(x_abs, y_sign) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // 8.6 下一个可表示数函数 vdNextAfter
    cout << "--- 8.6 下一个可表示数函数 (vdNextAfter) ---" << endl;
    vector<double> y_next(n);
    for (int i = 0; i < n; ++i) {
        y_next[i] = (double)(i + 1) / n * 2.0;  // 0 到 2
    }
    start = high_resolution_clock::now();
    vdNextAfter(n, x_abs.data(), y_next.data(), result.data());
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    print_array("nextafter(x_abs, y_next) (前10个)", result.data(), n, 10);
    cout << "计算耗时: " << duration.count() << " 微秒" << endl << endl;

    // ========== 精度模式 ==========
    cout << "\n=== VML 精度模式 ===" << endl;
    cout << "VML提供三种精度模式，在性能和精度之间权衡：" << endl;
    cout << "  - VML_HA (High Accuracy): 高精度模式，最高精度，较慢" << endl;
    cout << "  - VML_LA (Low Accuracy): 低精度模式，较低精度，较快" << endl;
    cout << "  - VML_EP (Enhanced Performance): 增强性能模式，平衡精度和性能" << endl;
    cout << "  - VML_TA (Target Accuracy): 目标精度模式，根据目标精度自动选择" << endl << endl;

    // 示例：精度模式设置
    cout << "--- 精度模式设置示例 ---" << endl;
    {
        const int n_acc = 1000;
        vector<double> x_acc(n_acc);
        vector<double> result_ha(n_acc), result_la(n_acc);

        for (int i = 0; i < n_acc; ++i) {
            x_acc[i] = (double)(i + 1) / n_acc * M_PI;
        }

        // 设置高精度模式
        vmlSetMode(VML_HA | VML_ERRMODE_IGNORE);
        auto start_acc = high_resolution_clock::now();
        vdSin(n_acc, x_acc.data(), result_ha.data());
        auto end_acc = high_resolution_clock::now();
        auto duration_ha = duration_cast<microseconds>(end_acc - start_acc);

        // 设置低精度模式
        vmlSetMode(VML_LA | VML_ERRMODE_IGNORE);
        start_acc = high_resolution_clock::now();
        vdSin(n_acc, x_acc.data(), result_la.data());
        end_acc = high_resolution_clock::now();
        auto duration_la = duration_cast<microseconds>(end_acc - start_acc);

        cout << "精度模式对比 (n=" << n_acc << "):" << endl;
        cout << "高精度模式 (HA) 耗时: " << duration_ha.count() << " 微秒" << endl;
        cout << "低精度模式 (LA) 耗时: " << duration_la.count() << " 微秒" << endl;
        if (duration_ha.count() > 0) {
            cout << "性能提升: " << (double)duration_ha.count() / duration_la.count() << "x" << endl;
        }

        // 计算精度差异
        double max_diff_acc = 0.0;
        for (int i = 0; i < n_acc; ++i) {
            double diff = abs(result_ha[i] - result_la[i]);
            max_diff_acc = max(max_diff_acc, diff);
        }
        cout << "最大精度差异: " << max_diff_acc << endl;
        cout << "注意：低精度模式速度更快，但精度略低" << endl << endl;

        // 恢复默认模式
        vmlSetMode(VML_EP | VML_ERRMODE_IGNORE);
    }

    // ========== SIMD优化 ==========
    cout << "\n=== SIMD优化 ===" << endl;
    cout << "VML自动利用SIMD指令集进行向量化：" << endl;
    cout << "  - SSE/SSE2: 128位向量化" << endl;
    cout << "  - AVX/AVX2: 256位向量化" << endl;
    cout << "  - AVX-512: 512位向量化（支持的CPU）" << endl;
    cout << "  - 多线程并行：自动利用多核CPU" << endl;
    cout << "  - 批量处理优化：针对大规模向量运算优化" << endl << endl;

    // 性能对比: VML vs 标准库
    cout << "--- SIMD优化性能对比: VML vs 标准库 ---" << endl;

    const int test_size = 10000000;
    vector<double> test_x(test_size);
    vector<double> test_result_vml(test_size);
    vector<double> test_result_std(test_size);

    for (int i = 0; i < test_size; ++i) {
        test_x[i] = (double)(i + 1) / test_size * M_PI;
    }

    // VML计算
    start = high_resolution_clock::now();
    vdSin(test_size, test_x.data(), test_result_vml.data());
    end = high_resolution_clock::now();
    auto vml_time = duration_cast<milliseconds>(end - start);

    // 标准库计算
    start = high_resolution_clock::now();
    for (int i = 0; i < test_size; ++i) {
        test_result_std[i] = sin(test_x[i]);
    }
    end = high_resolution_clock::now();
    auto std_time = duration_cast<milliseconds>(end - start);

    cout << "向量长度: " << test_size << endl;
    cout << "VML计算耗时: " << vml_time.count() << " 毫秒" << endl;
    cout << "标准库计算耗时: " << std_time.count() << " 毫秒" << endl;
    cout << "加速比: " << (double)std_time.count() / vml_time.count() << "x" << endl;

    // 验证结果一致性
    double max_diff = 0.0;
    for (int i = 0; i < min(1000, test_size); ++i) {
        double diff = abs(test_result_vml[i] - test_result_std[i]);
        max_diff = max(max_diff, diff);
    }
    cout << "最大误差 (前1000个元素): " << max_diff << endl;

    // ========== 批量处理优化 ==========
    cout << "\n=== 批量处理优化 ===" << endl;
    {
        const int batch_size = 100;
        const int n_batch = 10000;
        vector<double> batch_x(n_batch);
        vector<double> batch_result(n_batch);

        for (int i = 0; i < n_batch; ++i) {
            batch_x[i] = (double)(i + 1) / n_batch * M_PI;
        }

        // 批量处理：多次调用VML函数
        auto start_batch = high_resolution_clock::now();
        for (int b = 0; b < batch_size; ++b) {
            vdSin(n_batch, batch_x.data(), batch_result.data());
        }
        auto end_batch = high_resolution_clock::now();
        auto duration_batch = duration_cast<milliseconds>(end_batch - start_batch);

        cout << "批量处理性能测试 (batch=" << batch_size << ", n=" << n_batch << "):" << endl;
        cout << "总耗时: " << duration_batch.count() << " 毫秒" << endl;
        cout << "平均每次: " << duration_batch.count() / (double)batch_size << " 毫秒" << endl;
        cout << "VML的批量处理充分利用SIMD和缓存，性能优异" << endl << endl;
    }

    // ========== 错误处理模式 ==========
    cout << "\n=== 错误处理模式 ===" << endl;
    cout << "VML提供多种错误处理模式：" << endl;
    cout << "  - VML_ERRMODE_IGNORE: 忽略错误" << endl;
    cout << "  - VML_ERRMODE_ERRNO: 设置errno" << endl;
    cout << "  - VML_ERRMODE_CALLBACK: 使用回调函数" << endl;
    cout << "  - VML_ERRMODE_EXCEPT: 抛出异常" << endl << endl;

    // 示例：错误处理
    {
        const int n_err = 10;
        vector<double> x_err = {-1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        vector<double> result_err(n_err);

        // 设置错误处理模式
        vmlSetMode(VML_EP | VML_ERRMODE_IGNORE);

        // 计算sqrt（包含负数，会产生错误）
        vdSqrt(n_err, x_err.data(), result_err.data());
        cout << "错误处理示例（sqrt包含负数）:" << endl;
        print_array("sqrt(x_err) (负数会产生NaN)", result_err.data(), n_err, n_err);
        cout << "注意：VML会根据错误处理模式处理错误值" << endl << endl;
    }

    cout << "\n=== 总结 ===" << endl;
    cout << "MKL VML提供了高度优化的向量数学函数，具有以下优势：" << endl;
    cout << "  1. 完整的数学函数库：" << endl;
    cout << "     - 算术函数：Add, Sub, Mul, Div" << endl;
    cout << "     - 幂和根函数：Pow, Sqrt, Cbrt, Inv, InvSqrt" << endl;
    cout << "     - 指数和对数函数：Exp, Ln, Log10, Log2, Exp2, Exp10, Log1p, Expm1" << endl;
    cout << "     - 三角函数：Sin, Cos, Tan, Asin, Acos, Atan, Atan2, SinCos" << endl;
    cout << "     - 双曲函数：Sinh, Cosh, Tanh, Asinh, Acosh, Atanh" << endl;
    cout << "     - 特殊函数：Erf, Erfc, Gamma, LGamma, TGamma" << endl;
    cout << "     - 舍入函数：Floor, Ceil, Round, Trunc, NearbyInt" << endl;
    cout << "     - 杂项函数：Abs, Fabs, Fmod, Remainder, CopySign, NextAfter" << endl;
    cout << "  2. 向量运算：高效的向量级元素操作" << endl;
    cout << "  3. SIMD优化：自动利用SSE/AVX/AVX-512指令集" << endl;
    cout << "  4. 多线程并行：自动利用多核CPU" << endl;
    cout << "  5. 精度模式：支持高精度(HA)、低精度(LA)、增强性能(EP)等多种模式" << endl;
    cout << "  6. 批量处理：针对大规模向量运算优化" << endl;
    cout << "  7. 错误处理：灵活的错误处理机制" << endl;
    cout << "  8. 性能优异：相比标准库函数有显著性能提升" << endl;

    cin.get();
    return 0;
}
