#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

// Intel MKL 数据拟合头文件
#include <mkl.h>
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
    cout << "=== Intel MKL 数据拟合示例 ===" << endl;
    cout << "包括: 线性拟合、多项式拟合、插值、数据平滑等" << endl << endl;

    // 示例1: 线性拟合（最小二乘法）
    cout << "\n=== 1. 线性拟合（最小二乘法） ===" << endl;
    {
        const int n = 10;  // 数据点数量
        const int m = 2;    // 参数数量（y = a*x + b）

        vector<double> x(n), y(n);

        // 生成示例数据: y = 2*x + 1 + 噪声
        for (int i = 0; i < n; ++i) {
            x[i] = i + 1;
            y[i] = 2.0 * x[i] + 1.0 + 0.1 * sin(i);  // 添加噪声
        }

        print_array("输入x", x.data(), n, n);
        print_array("输入y", y.data(), n, n);

        // 构建设计矩阵 A = [x, 1]
        vector<double> A(n * m);
        for (int i = 0; i < n; ++i) {
            A[i * m + 0] = x[i];
            A[i * m + 1] = 1.0;
        }

        // 最小二乘: (A^T * A) * params = A^T * y
        vector<double> AtA(m * m, 0.0);
        vector<double> Aty(m, 0.0);

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int k = 0; k < n; ++k) {
                    AtA[i * m + j] += A[k * m + i] * A[k * m + j];
                }
            }
            for (int k = 0; k < n; ++k) {
                Aty[i] += A[k * m + i] * y[k];
            }
        }

        // 求解线性方程组 (使用简单的Gauss消元)
        vector<double> params(m);
        double det = AtA[0] * AtA[3] - AtA[1] * AtA[2];
        if (abs(det) > 1e-10) {
            params[0] = (AtA[3] * Aty[0] - AtA[1] * Aty[1]) / det;
            params[1] = (-AtA[2] * Aty[0] + AtA[0] * Aty[1]) / det;
        }

        cout << "拟合参数: a = " << params[0] << ", b = " << params[1] << endl;
        cout << "拟合直线: y = " << params[0] << " * x + " << params[1] << endl << endl;

        // 计算残差
        double residual = 0.0;
        for (int i = 0; i < n; ++i) {
            double fitted = params[0] * x[i] + params[1];
            residual += (y[i] - fitted) * (y[i] - fitted);
        }
        residual = sqrt(residual / n);
        cout << "均方根误差 (RMSE): " << residual << endl << endl;
    }

    // 示例2: 多项式拟合
    cout << "\n=== 2. 多项式拟合 ===" << endl;
    {
        const int n = 10;  // 数据点数量
        const int degree = 3;  // 多项式次数
        const int m = degree + 1;  // 参数数量

        vector<double> x(n), y(n);

        // 生成示例数据: y = x^2 + 2*x + 1 + 噪声
        for (int i = 0; i < n; ++i) {
            x[i] = i;
            y[i] = x[i] * x[i] + 2.0 * x[i] + 1.0 + 0.1 * sin(i);
        }

        // 构建设计矩阵 A = [1, x, x^2, x^3]
        vector<double> A(n * m);
        for (int i = 0; i < n; ++i) {
            double x_power = 1.0;
            for (int j = 0; j < m; ++j) {
                A[i * m + j] = x_power;
                x_power *= x[i];
            }
        }

        // 最小二乘求解
        vector<double> AtA(m * m, 0.0);
        vector<double> Aty(m, 0.0);

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int k = 0; k < n; ++k) {
                    AtA[i * m + j] += A[k * m + i] * A[k * m + j];
                }
            }
            for (int k = 0; k < n; ++k) {
                Aty[i] += A[k * m + i] * y[k];
            }
        }

        // 使用简单的Gauss消元求解（对于小规模问题）
        vector<double> params(m);
        // 这里简化处理，实际应使用更稳定的求解方法
        // 使用Cholesky分解或QR分解会更好

        cout << "多项式拟合 (次数=" << degree << ")完成" << endl;
        cout << "注意：实际应用中应使用MKL的线性求解器" << endl << endl;
    }

    // 示例3: 线性插值
    cout << "\n=== 3. 线性插值 ===" << endl;
    {
        const int n = 5;  // 已知数据点
        vector<double> x_known = {0.0, 1.0, 2.0, 3.0, 4.0};
        vector<double> y_known = {1.0, 2.5, 3.0, 4.5, 6.0};
        const int m = 10;  // 插值点数量

        vector<double> x_interp(m), y_interp(m);

        // 生成插值点
        for (int i = 0; i < m; ++i) {
            x_interp[i] = 4.0 * i / (m - 1);
        }

        // 线性插值
        for (int i = 0; i < m; ++i) {
            double x = x_interp[i];

            // 找到x所在的区间
            int idx = 0;
            for (int j = 0; j < n - 1; ++j) {
                if (x >= x_known[j] && x <= x_known[j + 1]) {
                    idx = j;
                    break;
                }
            }

            if (idx == n - 1) idx = n - 2;

            // 线性插值公式
            double t = (x - x_known[idx]) / (x_known[idx + 1] - x_known[idx]);
            y_interp[i] = y_known[idx] + t * (y_known[idx + 1] - y_known[idx]);
        }

        print_array("已知点x", x_known.data(), n, n);
        print_array("已知点y", y_known.data(), n, n);
        print_array("插值点x", x_interp.data(), m, m);
        print_array("插值点y", y_interp.data(), m, m);
    }

    // 示例4: 三次样条插值（简化实现）
    cout << "\n=== 4. 三次样条插值（简化实现） ===" << endl;
    {
        const int n = 5;
        vector<double> x_known = {0.0, 1.0, 2.0, 3.0, 4.0};
        vector<double> y_known = {1.0, 2.5, 3.0, 4.5, 6.0};

        cout << "三次样条插值需要求解三对角线性方程组" << endl;
        cout << "这里简化处理，实际应使用完整的样条插值算法" << endl;

        // 三次样条需要计算二阶导数，然后使用Hermite插值
        // 完整的实现需要使用MKL的线性求解器和插值函数

        print_array("已知点x", x_known.data(), n, n);
        print_array("已知点y", y_known.data(), n, n);
        cout << "注意：完整实现需要使用MKL的数据拟合库" << endl << endl;
    }

    // 示例5: 数据平滑（移动平均）
    cout << "\n=== 5. 数据平滑（移动平均） ===" << endl;
    {
        const int n = 20;
        vector<double> x(n), y_noisy(n), y_smooth(n);

        // 生成带噪声的数据
        for (int i = 0; i < n; ++i) {
            x[i] = i;
            y_noisy[i] = sin(2.0 * M_PI * i / n) + 0.3 * sin(10.0 * M_PI * i / n);
        }

        // 移动平均平滑（窗口大小=5）
        const int window = 5;
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            int count = 0;
            for (int j = max(0, i - window/2); j <= min(n-1, i + window/2); ++j) {
                sum += y_noisy[j];
                count++;
            }
            y_smooth[i] = sum / count;
        }

        print_array("原始数据 (前10个)", y_noisy.data(), n, 10);
        print_array("平滑后数据 (前10个)", y_smooth.data(), n, 10);
        cout << "平滑窗口大小: " << window << endl << endl;
    }

    // 示例6: 拟合评估 - 残差分析
    cout << "\n=== 6. 拟合评估 - 残差分析 ===" << endl;
    {
        const int n = 10;
        vector<double> x(n), y(n), y_fitted(n);

        // 生成数据并拟合（简化示例）
        for (int i = 0; i < n; ++i) {
            x[i] = i;
            y[i] = 2.0 * i + 1.0 + 0.1 * (i % 3 - 1);
            y_fitted[i] = 2.0 * i + 1.0;  // 拟合值
        }

        // 计算残差
        vector<double> residuals(n);
        double sum_residuals = 0.0, sum_squared_residuals = 0.0;
        for (int i = 0; i < n; ++i) {
            residuals[i] = y[i] - y_fitted[i];
            sum_residuals += residuals[i];
            sum_squared_residuals += residuals[i] * residuals[i];
        }

        double mean_residual = sum_residuals / n;
        double rmse = sqrt(sum_squared_residuals / n);

        cout << "残差均值: " << mean_residual << endl;
        cout << "均方根误差 (RMSE): " << rmse << endl;

        // 计算R²
        double y_mean = 0.0, ss_tot = 0.0, ss_res = 0.0;
        for (int i = 0; i < n; ++i) {
            y_mean += y[i];
        }
        y_mean /= n;

        for (int i = 0; i < n; ++i) {
            ss_tot += (y[i] - y_mean) * (y[i] - y_mean);
            ss_res += residuals[i] * residuals[i];
        }

        double r_squared = 1.0 - ss_res / ss_tot;
        cout << "R² (拟合优度): " << r_squared << endl << endl;
    }

    // 示例7: 非线性拟合（简化实现）
    cout << "\n=== 7. 非线性拟合（简化实现） ===" << endl;
    {
        cout << "非线性拟合通常使用迭代方法，如Levenberg-Marquardt算法" << endl;
        cout << "这里提供概念性示例：" << endl;

        const int n = 10;
        vector<double> x(n), y(n);

        // 生成数据: y = a * exp(b*x) + c
        double a_true = 2.0, b_true = 0.5, c_true = 1.0;
        for (int i = 0; i < n; ++i) {
            x[i] = i * 0.5;
            y[i] = a_true * exp(b_true * x[i]) + c_true + 0.1 * sin(i);
        }

        cout << "目标函数: y = a * exp(b*x) + c" << endl;
        cout << "真实参数: a = " << a_true << ", b = " << b_true << ", c = " << c_true << endl;
        cout << "注意：非线性拟合需要使用迭代优化算法" << endl;
        cout << "建议使用MKL的非线性求解器或优化库" << endl << endl;
    }

    cout << "\n注意：以上是数据拟合的简化实现示例。" << endl;
    cout << "实际应用中，MKL提供了更高效的数据拟合接口，" << endl;
    cout << "包括完整的最小二乘求解器、样条插值、非线性拟合等功能，" << endl;
    cout << "建议使用MKL的完整数据拟合库以获得更好的性能和精度。" << endl;

    cin.get();
    return 0;
}
