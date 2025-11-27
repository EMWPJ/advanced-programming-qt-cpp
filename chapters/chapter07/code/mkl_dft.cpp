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

// Intel MKL头文件
#include <mkl.h>
#include <mkl_dfti.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_complex_array(const char* name, const complex<double>* arr, int size, int max_print = 8) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << "(" << arr[i].real() << ", " << arr[i].imag() << ")";
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

// 计算DFT的浮点运算次数（FLOPs）
// 对于N点复数FFT，大约需要 5N*log2(N) 次浮点运算
// 对于N点实数FFT，大约需要 2.5N*log2(N) 次浮点运算
double calculate_dft_flops(int n, bool is_complex = true) {
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

// 蝶形算法FFT实现（基于Cooley-Tukey算法的迭代版本）
// 蝶形运算是FFT的核心：对于两个输入a和b，输出为 a+W*b 和 a-W*b
void fft_butterfly(vector<complex<double>>& x) {
    int n = static_cast<int>(x.size());

    // 第一步：位反转置换（Bit-reversal permutation）
    // 将数据重新排列，使得后续的蝶形运算可以按顺序进行
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            swap(x[i], x[j]);
        }
    }

    // 第二步：逐级进行蝶形运算
    // 从最小的蝶形（长度为2）开始，逐步合并到完整的FFT
    for (int len = 2; len <= n; len <<= 1) {
        // 计算旋转因子（Twiddle factor）的基础角度
        double ang = -2.0 * M_PI / len;
        complex<double> wlen(cos(ang), sin(ang));  // e^(-2πi/len)

        // 对每一组数据进行蝶形运算
        for (int i = 0; i < n; i += len) {
            complex<double> w(1);  // 旋转因子，初始为1
            // 执行蝶形运算：对每一对元素进行运算
            for (int j = 0; j < len/2; ++j) {
                // 蝶形运算的核心：
                // u = x[i+j] (上分支)
                // v = x[i+j+len/2] * w (下分支乘以旋转因子)
                // 输出：x[i+j] = u + v, x[i+j+len/2] = u - v
                complex<double> u = x[i + j];                    // 上分支
                complex<double> v = x[i + j + len/2] * w;       // 下分支乘以旋转因子
                x[i + j] = u + v;                                // 蝶形输出1
                x[i + j + len/2] = u - v;                        // 蝶形输出2
                w *= wlen;  // 更新旋转因子
            }
        }
    }
}

// 归一化FFT结果
void normalize_fft(vector<complex<double>>& x) {
    int n = static_cast<int>(x.size());
    double scale = 1.0 / n;
    for (int i = 0; i < n; ++i) {
        x[i] *= scale;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== Intel MKL DFT (离散傅里叶变换接口) 示例 ===" << endl;
    cout << "DFT模块提供统一的DFTI接口，支持灵活的配置和高效的变换" << endl;
    cout << "主要特性：" << endl;
    cout << "  1. DFT接口：DFTI接口，统一接口设计，配置描述符" << endl;
    cout << "  2. 一维DFT：前向/反向变换，实数/复数DFT，配置灵活" << endl;
    cout << "  3. 多维DFT：2D/3D/N维DFT，多维变换，批量处理" << endl;
    cout << "  4. 实数/复数DFT：R2C/C2R变换，高效存储，内存优化" << endl;
    cout << "  5. 高级特性：步长控制，偏移设置，数据布局，性能调优" << endl;
    cout << "  6. 性能优化：多线程并行，SIMD向量化，缓存优化" << endl << endl;

    // ========== DFTI接口说明 ==========
    cout << "\n=== DFTI接口 ===" << endl;
    cout << "DFTI (Discrete Fourier Transform Interface) 是MKL提供的统一DFT接口" << endl;
    cout << "主要特点：" << endl;
    cout << "  - 统一接口：所有DFT操作使用相同的接口设计" << endl;
    cout << "  - 配置描述符：使用DFTI_DESCRIPTOR_HANDLE描述符管理配置" << endl;
    cout << "  - 灵活配置：通过DftiSetValue设置各种参数" << endl;
    cout << "  - 三步流程：创建描述符 -> 提交描述符 -> 执行变换" << endl << endl;

    cout << "基本使用流程：" << endl;
    cout << "  1. DftiCreateDescriptor: 创建DFT描述符" << endl;
    cout << "  2. DftiSetValue: 设置各种参数（可选）" << endl;
    cout << "  3. DftiCommitDescriptor: 提交描述符，完成配置" << endl;
    cout << "  4. DftiComputeForward/DftiComputeBackward: 执行变换" << endl;
    cout << "  5. DftiFreeDescriptor: 释放描述符" << endl << endl;

    // 示例1: 一维复数DFT - 基本用法
    cout << "\n=== 1. 一维复数DFT ===" << endl;
    cout << "展示DFTI接口的基本使用：前向和反向变换" << endl;
    {
        const int n = 8;
        vector<complex<double>> input(n);
        vector<complex<double>> output(n);

        // 创建输入信号
        for (int i = 0; i < n; ++i) {
            input[i] = complex<double>(cos(2.0 * M_PI * i / n), sin(2.0 * M_PI * i / n));
        }

        print_complex_array("输入信号", input.data(), n);

        // 创建DFT描述符
        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status;

        status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);
        if (status != DFTI_NO_ERROR) {
            cout << "创建DFT描述符失败" << endl;
            return 1;
        }

        // 设置非原地操作（输入和输出使用不同的数组）
        status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
        // 设置缩放因子：前向DFT除以n（归一化），反向DFT使用默认的1/n缩放
        // 这样：前向输出 = DFT(input)/n，反向输出 = IDFT(前向输出)/n = input
        status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
        // 不设置BACKWARD_SCALE，使用默认值1.0/n

        // 提交描述符
        status = DftiCommitDescriptor(handle);
        if (status != DFTI_NO_ERROR) {
            cout << "提交DFT描述符失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        // 执行前向DFT
        auto start = high_resolution_clock::now();
        status = DftiComputeForward(handle,
                                   reinterpret_cast<double*>(input.data()),
                                   reinterpret_cast<double*>(output.data()));
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        if (status != DFTI_NO_ERROR) {
            cout << "DFT计算失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        print_complex_array("前向DFT结果", output.data(), n);
        cout << "前向DFT计算耗时: " << duration.count() << " 微秒" << endl;

        // 执行反向DFT（使用output作为输入，恢复原始信号）
        vector<complex<double>> recovered(n);
        status = DftiComputeBackward(handle,
                                    reinterpret_cast<double*>(output.data()),
                                    reinterpret_cast<double*>(recovered.data()));

        if (status == DFTI_NO_ERROR) {
            print_complex_array("反向DFT结果（应该恢复原始输入）", recovered.data(), n);
        }

        DftiFreeDescriptor(&handle);
    }

    // ========== 多维DFT ==========
    cout << "\n=== 多维DFT ===" << endl;
    cout << "DFTI接口支持2D、3D和更高维度的DFT变换" << endl << endl;

    // 示例2: 二维DFT
    cout << "--- 2. 二维DFT (2D) ---" << endl;
    {
        const int rows = 4, cols = 4;
        vector<complex<double>> input_2d(rows * cols);
        vector<complex<double>> output_2d(rows * cols);

        // 创建2D输入数据
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                double value = sin(2.0 * M_PI * i / rows) * cos(2.0 * M_PI * j / cols);
                input_2d[i * cols + j] = complex<double>(value, 0.0);
            }
        }

        print_complex_array("2D输入数据 (前8个)", input_2d.data(), min(8, rows*cols));

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG lengths[2] = {rows, cols};

        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 2, lengths);
        if (status != DFTI_NO_ERROR) {
            cout << "创建2D DFT描述符失败" << endl;
            return 1;
        }

        status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
        status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / (rows * cols));
        status = DftiCommitDescriptor(handle);

        if (status != DFTI_NO_ERROR) {
            cout << "2D DFT设置失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        // 执行2D DFT
        status = DftiComputeForward(handle,
                                   reinterpret_cast<double*>(input_2d.data()),
                                   reinterpret_cast<double*>(output_2d.data()));

        if (status != DFTI_NO_ERROR) {
            cout << "2D DFT计算失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        print_complex_array("2D DFT结果 (前8个)", output_2d.data(), min(8, rows*cols));

        DftiFreeDescriptor(&handle);
    }

    // 示例3: 三维DFT
    cout << "\n--- 3. 三维DFT (3D) ---" << endl;
    {
        const int dim1 = 3, dim2 = 3, dim3 = 3;
        vector<complex<double>> input_3d(dim1 * dim2 * dim3);
        vector<complex<double>> output_3d(dim1 * dim2 * dim3);

        // 创建3D输入数据
        for (int i = 0; i < dim1; ++i) {
            for (int j = 0; j < dim2; ++j) {
                for (int k = 0; k < dim3; ++k) {
                    int idx = i * dim2 * dim3 + j * dim3 + k;
                    double value = sin(2.0 * M_PI * i / dim1) *
                                  cos(2.0 * M_PI * j / dim2) *
                                  sin(2.0 * M_PI * k / dim3);
                    input_3d[idx] = complex<double>(value, 0.0);
                }
            }
        }

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG lengths[3] = {dim1, dim2, dim3};

        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 3, lengths);
        if (status != DFTI_NO_ERROR) {
            cout << "创建3D DFT描述符失败" << endl;
        } else {
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / (dim1 * dim2 * dim3));
            status = DftiCommitDescriptor(handle);

            if (status == DFTI_NO_ERROR) {
                status = DftiComputeForward(handle,
                                           reinterpret_cast<double*>(input_3d.data()),
                                           reinterpret_cast<double*>(output_3d.data()));

                if (status == DFTI_NO_ERROR) {
                    cout << "3D DFT计算成功 (维度: " << dim1 << "x" << dim2 << "x" << dim3 << ")" << endl;
                    print_complex_array("3D DFT结果 (前8个)", output_3d.data(), min(8, dim1*dim2*dim3));
                }
            }

            DftiFreeDescriptor(&handle);
        }
    }

    // ========== 实数/复数DFT ==========
    cout << "\n=== 实数/复数DFT ===" << endl;
    cout << "实数DFT (R2C/C2R) 的优势：" << endl;
    cout << "  - 高效存储：实数输入只需存储一半的复数输出（利用对称性）" << endl;
    cout << "  - 内存优化：减少内存使用和传输" << endl;
    cout << "  - 性能提升：针对实数数据优化" << endl << endl;

    // 示例4: 实数DFT (R2C/C2R)
    cout << "--- 4. 实数DFT (R2C/C2R) ---" << endl;
    {
        const int n = 8;
        vector<double> real_input = {1.0, 2.0, 3.0, 4.0, 4.0, 3.0, 2.0, 1.0};
        vector<double> complex_output(n + 2);  // 实数DFT输出需要额外空间

        print_real_array("实数输入", real_input.data(), n);

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_REAL, 1, n);
        if (status != DFTI_NO_ERROR) {
            cout << "创建实数DFT描述符失败" << endl;
            return 1;
        }

        // 设置非原地操作
        status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
        // 设置缩放因子：前向R2C除以n（归一化），反向C2R使用默认的1/n缩放
        // 这样：前向输出 = R2C(input)/n，反向输出 = C2R(前向输出)/n = input
        status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
        // 不设置BACKWARD_SCALE，使用默认值1.0/n
        status = DftiCommitDescriptor(handle);

        if (status != DFTI_NO_ERROR) {
            cout << "实数DFT设置失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        // 执行实数到复数DFT
        status = DftiComputeForward(handle, real_input.data(), complex_output.data());

        if (status != DFTI_NO_ERROR) {
            cout << "实数DFT计算失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        // 转换输出格式
        vector<complex<double>> r2c_result(n/2 + 1);
        for (int i = 0; i < n/2 + 1; ++i) {
            r2c_result[i] = complex<double>(complex_output[2*i], complex_output[2*i+1]);
        }

        print_complex_array("实数DFT结果 (R2C)", r2c_result.data(), static_cast<int>(r2c_result.size()));

        // 执行C2R逆变换（恢复原始输入）
        vector<double> recovered_input(n);
        status = DftiComputeBackward(handle, complex_output.data(), recovered_input.data());

        if (status == DFTI_NO_ERROR) {
            print_real_array("C2R逆DFT结果（应该恢复原始输入）", recovered_input.data(), n);
        }

        DftiFreeDescriptor(&handle);
    }

    // ========== 高级特性 ==========
    cout << "\n=== 高级特性 ===" << endl;
    cout << "DFTI接口提供丰富的高级特性：" << endl;
    cout << "  - 步长控制：支持非连续数据访问" << endl;
    cout << "  - 偏移设置：支持数据偏移" << endl;
    cout << "  - 数据布局：支持行主序/列主序" << endl;
    cout << "  - 性能调优：多种优化选项" << endl << endl;

    // 示例5: 高级特性 - 步长控制
    cout << "--- 5. 步长控制 ===" << endl;
    cout << "注意：步长控制在MKL DFT中需要精确配置，这里展示基本概念" << endl;
    {
        const int n = 8;
        vector<complex<double>> input_stride(n);
        vector<complex<double>> output_stride(n);

        // 初始化输入数据
        for (int i = 0; i < n; ++i) {
            input_stride[i] = complex<double>(cos(2.0 * M_PI * i / n), sin(2.0 * M_PI * i / n));
        }

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);
        if (status != DFTI_NO_ERROR) {
            cout << "创建DFT描述符失败，跳过此示例" << endl;
        } else {
            // 设置非原地操作
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
            status = DftiCommitDescriptor(handle);

            if (status != DFTI_NO_ERROR) {
                cout << "DFT设置失败，跳过此示例" << endl;
                DftiFreeDescriptor(&handle);
            } else {
                // 执行DFT（使用标准连续数据，步长控制在实际应用中需要更复杂的配置）
                status = DftiComputeForward(handle,
                                           reinterpret_cast<double*>(input_stride.data()),
                                           reinterpret_cast<double*>(output_stride.data()));

                if (status != DFTI_NO_ERROR) {
                    cout << "DFT计算失败，跳过输出" << endl;
                } else {
                    cout << "步长控制示例完成（使用标准连续数据）" << endl;
                    print_complex_array("DFT结果 (前4个)", output_stride.data(), 4);
                }

                DftiFreeDescriptor(&handle);
            }
        }
    }

    // 示例6: 偏移设置
    cout << "\n--- 6. 偏移设置 ---" << endl;
    {
        const int n = 8;
        const int offset = 2;  // 数据偏移
        vector<complex<double>> large_array(n + offset * 2);
        vector<complex<double>> output_array(n + offset * 2);

        // 在偏移位置初始化数据
        for (int i = 0; i < n; ++i) {
            large_array[offset + i] = complex<double>(cos(2.0 * M_PI * i / n), sin(2.0 * M_PI * i / n));
        }

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);

        if (status == DFTI_NO_ERROR) {
            // 设置非原地操作
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
            // 设置输入和输出的偏移
            MKL_LONG input_strides[2] = {0, 1};  // {偏移, 步长}
            MKL_LONG output_strides[2] = {0, 1};

            status = DftiSetValue(handle, DFTI_INPUT_STRIDES, input_strides);
            status = DftiSetValue(handle, DFTI_OUTPUT_STRIDES, output_strides);
            status = DftiSetValue(handle, DFTI_INPUT_DISTANCE, offset);
            status = DftiSetValue(handle, DFTI_OUTPUT_DISTANCE, offset);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
            status = DftiCommitDescriptor(handle);

            if (status == DFTI_NO_ERROR) {
                status = DftiComputeForward(handle,
                                           reinterpret_cast<double*>(large_array.data() + offset),
                                           reinterpret_cast<double*>(output_array.data() + offset));

                if (status == DFTI_NO_ERROR) {
                    cout << "带偏移的DFT计算成功 (偏移=" << offset << ")" << endl;
                    print_complex_array("DFT结果 (从偏移位置开始)", output_array.data() + offset, n);
                }
            }

            DftiFreeDescriptor(&handle);
        }
    }

    // 示例7: 数据布局控制
    cout << "\n--- 7. 数据布局控制 ---" << endl;
    {
        const int rows = 4, cols = 4;
        vector<complex<double>> input_layout(rows * cols);
        vector<complex<double>> output_layout(rows * cols);

        // 初始化数据
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                input_layout[i * cols + j] = complex<double>(i * cols + j, 0.0);
            }
        }

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG lengths[2] = {rows, cols};
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 2, lengths);

        if (status == DFTI_NO_ERROR) {
            // 设置非原地操作
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
            // 设置行主序布局（C风格）
            status = DftiSetValue(handle, DFTI_PACKED_FORMAT, DFTI_CCS_FORMAT);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / (rows * cols));
            status = DftiCommitDescriptor(handle);

            if (status == DFTI_NO_ERROR) {
                status = DftiComputeForward(handle,
                                           reinterpret_cast<double*>(input_layout.data()),
                                           reinterpret_cast<double*>(output_layout.data()));

                if (status == DFTI_NO_ERROR) {
                    cout << "2D DFT计算成功（行主序布局）" << endl;
                }
            }

            DftiFreeDescriptor(&handle);
        }
    }

    // 示例8: 批量处理
    cout << "\n--- 8. 批量处理 ---" << endl;
    {
        const int n = 4;  // 每个DFT的长度
        const int batch = 3;  // 批量数量

        vector<complex<double>> batch_input(n * batch);
        vector<complex<double>> batch_output(n * batch);

        // 初始化批量数据
        for (int b = 0; b < batch; ++b) {
            for (int i = 0; i < n; ++i) {
                double phase = 2.0 * M_PI * (i + b) / n;
                batch_input[b * n + i] = complex<double>(cos(phase), sin(phase));
            }
        }

        cout << "批量输入 (batch=" << batch << ", n=" << n << "):" << endl;
        for (int b = 0; b < batch; ++b) {
            cout << "Batch " << b << ": ";
            for (int i = 0; i < n; ++i) {
                cout << "(" << batch_input[b*n+i].real() << "," << batch_input[b*n+i].imag() << ") ";
            }
            cout << endl;
        }
        cout << endl;

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);
        if (status != DFTI_NO_ERROR) {
            cout << "创建DFT描述符失败" << endl;
            return 1;
        }

        // 设置非原地操作
        status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
        // 设置批量处理
        status = DftiSetValue(handle, DFTI_NUMBER_OF_TRANSFORMS, batch);
        status = DftiSetValue(handle, DFTI_INPUT_DISTANCE, n);
        status = DftiSetValue(handle, DFTI_OUTPUT_DISTANCE, n);
        status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
        status = DftiCommitDescriptor(handle);

        if (status != DFTI_NO_ERROR) {
            cout << "批量DFT设置失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        // 执行批量DFT
        status = DftiComputeForward(handle,
                                   reinterpret_cast<double*>(batch_input.data()),
                                   reinterpret_cast<double*>(batch_output.data()));

        if (status != DFTI_NO_ERROR) {
            cout << "批量DFT计算失败" << endl;
            DftiFreeDescriptor(&handle);
            return 1;
        }

        cout << "批量输出:" << endl;
        for (int b = 0; b < batch; ++b) {
            cout << "Batch " << b << ": ";
            for (int i = 0; i < n; ++i) {
                cout << "(" << batch_output[b*n+i].real() << "," << batch_output[b*n+i].imag() << ") ";
            }
            cout << endl;
        }

        DftiFreeDescriptor(&handle);
    }

    // ========== 性能优化 ==========
    cout << "\n=== 性能优化 ===" << endl;
    cout << "MKL DFT的性能优化特性：" << endl;
    cout << "  1. 多线程并行：自动利用多核CPU" << endl;
    cout << "  2. SIMD向量化：充分利用AVX/AVX2/AVX-512指令" << endl;
    cout << "  3. 缓存优化：针对CPU缓存层次结构优化" << endl;
    cout << "  4. 内存对齐：自动处理内存对齐" << endl << endl;

    // 示例9: 性能测试（包含GFLOPS）
    cout << "--- 9. 性能测试（包含GFLOPS） ---" << endl;
    {
        const int n_perf = 65536 * 64;
        const int iterations = 1000;
        vector<complex<double>> input_perf(n_perf);
        vector<complex<double>> output_perf(n_perf);

        // 初始化
        for (int i = 0; i < n_perf; ++i) {
            input_perf[i] = complex<double>(cos(2.0 * M_PI * i / n_perf), sin(2.0 * M_PI * i / n_perf));
        }

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n_perf);

        if (status == DFTI_NO_ERROR) {
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n_perf);
            status = DftiSetValue(handle, DFTI_BACKWARD_SCALE, 1.0 / n_perf);

            // 性能优化选项
            status = DftiSetValue(handle, DFTI_THREAD_LIMIT, 4);  // 使用4个线程
            status = DftiCommitDescriptor(handle);

            if (status == DFTI_NO_ERROR) {
                // 预热（避免首次调用的开销影响性能测试）
                for (int i = 0; i < 10; ++i) {
                    DftiComputeForward(handle,
                                     reinterpret_cast<double*>(input_perf.data()),
                                     reinterpret_cast<double*>(output_perf.data()));
                }

                // 正式性能测试
                auto start = high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                    DftiComputeForward(handle,
                                     reinterpret_cast<double*>(input_perf.data()),
                                     reinterpret_cast<double*>(output_perf.data()));
                }
                auto end = high_resolution_clock::now();

                // 使用更精确的时间单位（微秒）
                auto duration_us = duration_cast<microseconds>(end - start);
                auto duration_ms = duration_cast<milliseconds>(end - start);
                double time_seconds = duration_us.count() / 1e6;

                // 计算浮点运算次数和GFLOPS
                double flops_per_dft = calculate_dft_flops(n_perf, true);  // 复数DFT
                double total_flops = flops_per_dft * iterations;
                double gflops = calculate_gflops(total_flops, time_seconds);

                cout << "大规模DFT性能测试 (n=" << n_perf << ", " << iterations << "次迭代):" << endl;
                cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                cout << "时间统计:" << endl;
                cout << "  总耗时: " << duration_ms.count() << " 毫秒 (" << duration_us.count() << " 微秒)" << endl;
                cout << "  平均每次: " << duration_us.count() / (double)iterations << " 微秒" << endl;
                cout << "  吞吐量: " << (iterations * 1000.0) / duration_ms.count() << " DFT/秒" << endl;
                cout << endl;
                cout << "性能指标:" << endl;
                cout << "  每次DFT运算量: " << flops_per_dft / 1e6 << " MFLOPs" << endl;
                cout << "  总运算量: " << total_flops / 1e9 << " GFLOPs" << endl;
                cout << "  性能: " << gflops << " GFLOPS" << endl;
                cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                cout << "MKL DFT充分利用多线程和SIMD指令，性能优异" << endl << endl;
            }

            DftiFreeDescriptor(&handle);
        }
    }

    // 示例10: 多规模性能对比测试
    cout << "\n--- 10. 多规模性能对比测试（GFLOPS） ---" << endl;
    {
        vector<int> test_sizes = {256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304};
        const int iterations = 500;

        cout << "测试不同规模的DFT性能（" << iterations << "次迭代）:" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "规模(N)\t\t时间(ms)\t平均(μs)\t吞吐量(DFT/s)\tGFLOPS" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

        for (int n : test_sizes) {
            vector<complex<double>> input_test(n);
            vector<complex<double>> output_test(n);

            // 初始化
            for (int i = 0; i < n; ++i) {
                input_test[i] = complex<double>(cos(2.0 * M_PI * i / n), sin(2.0 * M_PI * i / n));
            }

            DFTI_DESCRIPTOR_HANDLE handle;
            MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);

            if (status == DFTI_NO_ERROR) {
                status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
                status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
                status = DftiSetValue(handle, DFTI_THREAD_LIMIT, 4);  // 使用4个线程
                status = DftiCommitDescriptor(handle);

                if (status == DFTI_NO_ERROR) {
                    // 预热
                    for (int i = 0; i < 5; ++i) {
                        DftiComputeForward(handle,
                                         reinterpret_cast<double*>(input_test.data()),
                                         reinterpret_cast<double*>(output_test.data()));
                    }

                    // 性能测试
                    auto start = high_resolution_clock::now();
                    for (int i = 0; i < iterations; ++i) {
                        DftiComputeForward(handle,
                                         reinterpret_cast<double*>(input_test.data()),
                                         reinterpret_cast<double*>(output_test.data()));
                    }
                    auto end = high_resolution_clock::now();

                    auto duration_us = duration_cast<microseconds>(end - start);
                    auto duration_ms = duration_cast<milliseconds>(end - start);
                    double time_seconds = duration_us.count() / 1e6;

                    double flops_per_dft = calculate_dft_flops(n, true);
                    double total_flops = flops_per_dft * iterations;
                    double gflops = calculate_gflops(total_flops, time_seconds);

                    cout << n << "\t\t"
                         << duration_ms.count() << "\t\t"
                         << fixed << setprecision(2) << duration_us.count() / (double)iterations << "\t\t"
                         << fixed << setprecision(1) << (iterations * 1000.0) / duration_ms.count() << "\t\t"
                         << fixed << setprecision(3) << gflops << endl;
                }

                DftiFreeDescriptor(&handle);
            }
        }
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "注：GFLOPS值越高表示性能越好，MKL DFT针对不同规模进行了优化" << endl << endl;
    }

    // 示例11: 原地变换（内存优化）
    cout << "\n--- 11. 原地变换（内存优化） ---" << endl;
    {
        const int n = 8;
        vector<complex<double>> data(n);

        // 初始化
        for (int i = 0; i < n; ++i) {
            data[i] = complex<double>(cos(2.0 * M_PI * i / n), sin(2.0 * M_PI * i / n));
        }

        print_complex_array("原始数据", data.data(), n);

        DFTI_DESCRIPTOR_HANDLE handle;
        MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n);

        if (status == DFTI_NO_ERROR) {
            // 设置原地变换（输入和输出使用同一内存）
            status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_INPLACE);
            status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n);
            status = DftiCommitDescriptor(handle);

            if (status == DFTI_NO_ERROR) {
                status = DftiComputeForward(handle, reinterpret_cast<double*>(data.data()), nullptr);

                if (status == DFTI_NO_ERROR) {
                    cout << "原地DFT计算成功（节省内存）" << endl;
                    print_complex_array("DFT结果（原地）", data.data(), n);
                }
            }

            DftiFreeDescriptor(&handle);
        }
    }

    // ========== MKL DFT vs 蝶形算法FFT 性能对比 ==========
    cout << "\n=== MKL DFT vs 蝶形算法FFT 性能对比 ===" << endl;
    cout << "使用多个大规模FFT任务对比MKL DFT和自实现蝶形算法FFT的效率" << endl;
    cout << "蝶形算法是FFT的核心，通过逐级蝶形运算实现快速傅里叶变换" << endl << endl;

    // 测试多个不同规模的数据
    vector<int> test_sizes = {65536, 262144, 1048576, 4194304};  // 2^16, 2^18, 2^20, 2^22
    vector<string> size_names = {"64K (2^16)", "256K (2^18)", "1M (2^20)", "4M (2^22)"};
    const int iterations = 5;  // 每个规模的迭代次数

    cout << "测试配置：" << endl;
    cout << "  测试规模: ";
    for (size_t i = 0; i < test_sizes.size(); ++i) {
        cout << size_names[i];
        if (i < test_sizes.size() - 1) cout << ", ";
    }
    cout << endl;
    cout << "  每个规模迭代次数: " << iterations << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

    // 对每个规模进行测试
    for (size_t test_idx = 0; test_idx < test_sizes.size(); ++test_idx) {
        const int n_large = test_sizes[test_idx];

        cout << "\n【测试规模: " << size_names[test_idx] << " (N=" << n_large << ")】" << endl;

        // 创建测试数据
        vector<complex<double>> input_mkl(n_large);
        vector<complex<double>> output_mkl(n_large);
        vector<complex<double>> input_fft(n_large);
        vector<complex<double>> output_fft(n_large);

        // 初始化输入数据（相同的测试信号）
        cout << "  初始化数据中..." << flush;
        for (int i = 0; i < n_large; ++i) {
            double phase = 2.0 * M_PI * i / n_large;
            complex<double> value(cos(phase), sin(phase));
            input_mkl[i] = value;
            input_fft[i] = value;
        }
        cout << " 完成" << endl;

        // ========== MKL DFT 测试 ==========
        cout << "\n【MKL DFT 测试】" << endl;
        {
            DFTI_DESCRIPTOR_HANDLE handle;
            MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n_large);

            if (status == DFTI_NO_ERROR) {
                status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
                status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n_large);
                status = DftiSetValue(handle, DFTI_THREAD_LIMIT, 4);  // 使用4个线程
                status = DftiCommitDescriptor(handle);

                if (status == DFTI_NO_ERROR) {
                    // 预热
                    for (int i = 0; i < 3; ++i) {
                        DftiComputeForward(handle,
                                         reinterpret_cast<double*>(input_mkl.data()),
                                         reinterpret_cast<double*>(output_mkl.data()));
                    }

                    // 性能测试
                    cout << "  执行性能测试中..." << flush;
                    auto start = high_resolution_clock::now();
                    for (int i = 0; i < iterations; ++i) {
                        // 使用memcpy快速复制数据（对于大数据量更高效）
                        memcpy(input_mkl.data(), input_fft.data(), n_large * sizeof(complex<double>));
                        DftiComputeForward(handle,
                                         reinterpret_cast<double*>(input_mkl.data()),
                                         reinterpret_cast<double*>(output_mkl.data()));
                    }
                    auto end = high_resolution_clock::now();
                    cout << " 完成" << endl;

                    auto duration_us = duration_cast<microseconds>(end - start);
                    auto duration_ms = duration_cast<milliseconds>(end - start);
                    double time_seconds = duration_us.count() / 1e6;
                    double avg_time_ms = duration_ms.count() / (double)iterations;

                    double flops_per_fft = calculate_dft_flops(n_large, true);
                    double total_flops = flops_per_fft * iterations;
                    double gflops = calculate_gflops(total_flops, time_seconds);

                    cout << "  总耗时: " << duration_ms.count() << " 毫秒" << endl;
                    cout << "  平均每次: " << fixed << setprecision(3) << avg_time_ms << " 毫秒" << endl;
                    cout << "  吞吐量: " << fixed << setprecision(1) << (iterations * 1000.0) / duration_ms.count() << " FFT/秒" << endl;
                    cout << "  性能: " << fixed << setprecision(3) << gflops << " GFLOPS" << endl;

                    DftiFreeDescriptor(&handle);
                }
            }
        }

        // ========== 蝶形算法FFT测试 ==========
        cout << "\n【蝶形算法FFT测试（自实现）】" << endl;
        {
            // 预热
            cout << "  预热中..." << flush;
            for (int i = 0; i < 3; ++i) {
                memcpy(output_fft.data(), input_fft.data(), n_large * sizeof(complex<double>));
                fft_butterfly(output_fft);
                normalize_fft(output_fft);
            }
            cout << " 完成" << endl;

            // 性能测试
            cout << "  执行性能测试中..." << flush;
            auto start = high_resolution_clock::now();
            for (int i = 0; i < iterations; ++i) {
                // 使用memcpy快速复制数据
                memcpy(output_fft.data(), input_fft.data(), n_large * sizeof(complex<double>));
                fft_butterfly(output_fft);
                normalize_fft(output_fft);
            }
            auto end = high_resolution_clock::now();
            cout << " 完成" << endl;

            auto duration_us = duration_cast<microseconds>(end - start);
            auto duration_ms = duration_cast<milliseconds>(end - start);
            double time_seconds = duration_us.count() / 1e6;
            double avg_time_ms = duration_ms.count() / (double)iterations;

            double flops_per_fft = calculate_dft_flops(n_large, true);
            double total_flops = flops_per_fft * iterations;
            double gflops = calculate_gflops(total_flops, time_seconds);

            cout << "  总耗时: " << duration_ms.count() << " 毫秒" << endl;
            cout << "  平均每次: " << fixed << setprecision(3) << avg_time_ms << " 毫秒" << endl;
            cout << "  吞吐量: " << fixed << setprecision(1) << (iterations * 1000.0) / duration_ms.count() << " FFT/秒" << endl;
            cout << "  性能: " << fixed << setprecision(3) << gflops << " GFLOPS" << endl;
        }

        // ========== 结果验证（仅对较小数据量进行完整验证）==========
        if (n_large <= 262144) {  // 只对256K及以下的数据进行完整验证
            cout << "\n【结果验证】" << endl;
            cout << "  验证中..." << flush;
            {
                // 重新计算MKL结果用于验证
                vector<complex<double>> mkl_result(n_large);
                DFTI_DESCRIPTOR_HANDLE handle;
                MKL_LONG status = DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_COMPLEX, 1, n_large);
                if (status == DFTI_NO_ERROR) {
                    status = DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
                    status = DftiSetValue(handle, DFTI_FORWARD_SCALE, 1.0 / n_large);
                    status = DftiCommitDescriptor(handle);
                    if (status == DFTI_NO_ERROR) {
                        DftiComputeForward(handle,
                                         reinterpret_cast<double*>(input_mkl.data()),
                                         reinterpret_cast<double*>(mkl_result.data()));
                    }
                    DftiFreeDescriptor(&handle);
                }

                // 重新计算蝶形算法FFT结果用于验证
                memcpy(output_fft.data(), input_fft.data(), n_large * sizeof(complex<double>));
                fft_butterfly(output_fft);
                normalize_fft(output_fft);

                // 计算误差（对于大数据量，只验证前1000个点）
                int verify_count = min(n_large, 1000);
                double max_error = 0.0;
                double avg_error = 0.0;
                for (int i = 0; i < verify_count; ++i) {
                    complex<double> diff = mkl_result[i] - output_fft[i];
                    double error = abs(diff);
                    max_error = max(max_error, error);
                    avg_error += error;
                }
                avg_error /= verify_count;

                cout << " 完成" << endl;
                cout << "  验证点数: " << verify_count << " / " << n_large << endl;
                cout << "  最大误差: " << scientific << setprecision(3) << max_error << endl;
                cout << "  平均误差: " << scientific << setprecision(3) << avg_error << endl;
                if (max_error < 1e-10) {
                    cout << "  ✓ 结果验证通过（误差在可接受范围内）" << endl;
                } else {
                    cout << "  ⚠ 结果存在较大误差" << endl;
                }
            }
        } else {
            cout << "\n【结果验证】" << endl;
            cout << "  跳过验证（数据量过大，验证耗时较长）" << endl;
        }

        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    }

    // ========== 性能分析总结 ==========
    cout << "\n【性能分析总结】" << endl;
    cout << "MKL DFT的优势：" << endl;
    cout << "  1. 高度优化：使用SIMD指令（AVX/AVX2/AVX-512）进行向量化计算" << endl;
    cout << "  2. 多线程并行：自动利用多核CPU，并行执行蝶形运算" << endl;
    cout << "  3. 缓存优化：针对CPU缓存层次结构优化，减少内存访问延迟" << endl;
    cout << "  4. 内存对齐：自动处理内存对齐问题，提高访问效率" << endl;
    cout << "  5. 算法优化：使用高度优化的FFT算法实现，包括混合基数FFT等" << endl;
    cout << "\n蝶形算法FFT（自实现）的特点：" << endl;
    cout << "  1. 算法清晰：直接实现蝶形运算，易于理解FFT原理" << endl;
    cout << "  2. 无依赖：不需要外部库，纯C++实现" << endl;
    cout << "  3. 性能较低：未优化的实现，缺少SIMD和多线程优化" << endl;
    cout << "  4. 适合学习：适合理解蝶形运算和FFT算法原理" << endl;
    cout << "  5. 可定制：可以根据需求进行修改和优化" << endl << endl;

    cout << "\n=== 总结 ===" << endl;
    cout << "MKL DFT模块提供了统一的DFTI接口，具有以下优势：" << endl;
    cout << "  1. 统一接口：DFTI接口设计，易于使用" << endl;
    cout << "  2. 灵活配置：支持一维、多维、实数、复数等多种变换" << endl;
    cout << "  3. 高级特性：步长控制、偏移设置、数据布局等" << endl;
    cout << "  4. 内存优化：支持原地变换、高效存储等" << endl;
    cout << "  5. 性能优化：多线程、SIMD向量化、缓存优化" << endl;
    cout << "  6. 批量处理：支持批量变换，提高效率" << endl;

    cin.get();
    return 0;
}
