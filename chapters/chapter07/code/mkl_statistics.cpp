#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

// Intel MKL 统计函数头文件
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
    cout << "=== Intel MKL 统计函数示例 ===" << endl;
    cout << "包括: 描述性统计、相关性分析、假设检验等" << endl << endl;

    const int n = 1000;
    vector<double> data(n);

    // 生成示例数据（正态分布）
    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_MT19937, 12345);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, n, data.data(), 0.0, 1.0);
    vslDeleteStream(&stream);

    print_array("示例数据 (前10个)", data.data(), n, 10);

    // 示例1: 描述性统计 - 均值、方差、标准差
    cout << "\n=== 1. 描述性统计 ===" << endl;
    {
        double mean, variance, stddev;

        // 计算均值
        mean = 0.0;
        for (int i = 0; i < n; ++i) {
            mean += data[i];
        }
        mean /= n;

        // 计算方差和标准差
        variance = 0.0;
        for (int i = 0; i < n; ++i) {
            double diff = data[i] - mean;
            variance += diff * diff;
        }
        variance /= (n - 1);  // 样本方差
        stddev = sqrt(variance);

        cout << "均值: " << mean << endl;
        cout << "方差: " << variance << endl;
        cout << "标准差: " << stddev << endl << endl;
    }

    // 示例2: 偏度和峰度
    cout << "\n=== 2. 偏度和峰度 ===" << endl;
    {
        // 计算均值
        double mean = 0.0;
        for (int i = 0; i < n; ++i) {
            mean += data[i];
        }
        mean /= n;

        // 计算标准差
        double variance = 0.0;
        for (int i = 0; i < n; ++i) {
            double diff = data[i] - mean;
            variance += diff * diff;
        }
        variance /= (n - 1);
        double stddev = sqrt(variance);

        // 计算偏度
        double skewness = 0.0;
        for (int i = 0; i < n; ++i) {
            double z = (data[i] - mean) / stddev;
            skewness += z * z * z;
        }
        skewness /= n;

        // 计算峰度
        double kurtosis = 0.0;
        for (int i = 0; i < n; ++i) {
            double z = (data[i] - mean) / stddev;
            kurtosis += z * z * z * z;
        }
        kurtosis /= n;
        kurtosis -= 3.0;  // 超额峰度

        cout << "偏度: " << skewness << endl;
        cout << "峰度: " << kurtosis << endl << endl;
    }

    // 示例3: 中位数和分位数
    cout << "\n=== 3. 中位数和分位数 ===" << endl;
    {
        vector<double> sorted_data = data;
        sort(sorted_data.begin(), sorted_data.end());

        double median;
        if (n % 2 == 0) {
            median = (sorted_data[n/2 - 1] + sorted_data[n/2]) / 2.0;
        } else {
            median = sorted_data[n/2];
        }

        // 第一四分位数（25%）
        int q1_idx = n / 4;
        double q1 = sorted_data[q1_idx];

        // 第三四分位数（75%）
        int q3_idx = 3 * n / 4;
        double q3 = sorted_data[q3_idx];

        cout << "中位数: " << median << endl;
        cout << "第一四分位数 (Q1): " << q1 << endl;
        cout << "第三四分位数 (Q3): " << q3 << endl;
        cout << "四分位距 (IQR): " << q3 - q1 << endl << endl;
    }

    // 示例4: 相关性分析
    cout << "\n=== 4. 相关性分析 ===" << endl;
    {
        // 创建第二个数据集（与第一个相关）
        vector<double> data2(n);
        for (int i = 0; i < n; ++i) {
            data2[i] = 0.7 * data[i] + 0.3 * (i % 10) / 10.0;  // 部分相关
        }

        // 计算均值
        double mean1 = 0.0, mean2 = 0.0;
        for (int i = 0; i < n; ++i) {
            mean1 += data[i];
            mean2 += data2[i];
        }
        mean1 /= n;
        mean2 /= n;

        // 计算协方差
        double covariance = 0.0;
        for (int i = 0; i < n; ++i) {
            covariance += (data[i] - mean1) * (data2[i] - mean2);
        }
        covariance /= (n - 1);

        // 计算标准差
        double var1 = 0.0, var2 = 0.0;
        for (int i = 0; i < n; ++i) {
            double diff1 = data[i] - mean1;
            double diff2 = data2[i] - mean2;
            var1 += diff1 * diff1;
            var2 += diff2 * diff2;
        }
        var1 /= (n - 1);
        var2 /= (n - 1);
        double stddev1 = sqrt(var1);
        double stddev2 = sqrt(var2);

        // 计算皮尔逊相关系数
        double correlation = covariance / (stddev1 * stddev2);

        cout << "数据1均值: " << mean1 << ", 标准差: " << stddev1 << endl;
        cout << "数据2均值: " << mean2 << ", 标准差: " << stddev2 << endl;
        cout << "协方差: " << covariance << endl;
        cout << "皮尔逊相关系数: " << correlation << endl << endl;
    }

    // 示例5: 协方差矩阵
    cout << "\n=== 5. 协方差矩阵 ===" << endl;
    {
        const int m = 3;  // 变量数量
        vector<vector<double>> dataset(m, vector<double>(n));

        // 生成多个数据集
        VSLStreamStatePtr stream;
        for (int j = 0; j < m; ++j) {
            vslNewStream(&stream, VSL_BRNG_MT19937, 12345 + j);
            vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, n, dataset[j].data(), 0.0, 1.0);
            vslDeleteStream(&stream);
        }

        // 计算均值
        vector<double> means(m, 0.0);
        for (int j = 0; j < m; ++j) {
            for (int i = 0; i < n; ++i) {
                means[j] += dataset[j][i];
            }
            means[j] /= n;
        }

        // 计算协方差矩阵
        vector<vector<double>> cov_matrix(m, vector<double>(m, 0.0));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                double cov = 0.0;
                for (int k = 0; k < n; ++k) {
                    cov += (dataset[i][k] - means[i]) * (dataset[j][k] - means[j]);
                }
                cov_matrix[i][j] = cov / (n - 1);
            }
        }

        cout << "协方差矩阵 (" << m << "x" << m << "):" << endl;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << cov_matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }

    // 示例6: t检验（简化实现）
    cout << "\n=== 6. t检验（简化实现） ===" << endl;
    {
        // 创建两个样本
        vector<double> sample1(n / 2);
        vector<double> sample2(n / 2);

        VSLStreamStatePtr stream;
        vslNewStream(&stream, VSL_BRNG_MT19937, 11111);
        vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, n/2, sample1.data(), 0.0, 1.0);
        vslDeleteStream(&stream);

        vslNewStream(&stream, VSL_BRNG_MT19937, 22222);
        vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, n/2, sample2.data(), 0.5, 1.0);
        vslDeleteStream(&stream);

        // 计算均值
        double mean1 = 0.0, mean2 = 0.0;
        for (int i = 0; i < n/2; ++i) {
            mean1 += sample1[i];
            mean2 += sample2[i];
        }
        mean1 /= (n/2);
        mean2 /= (n/2);

        // 计算方差
        double var1 = 0.0, var2 = 0.0;
        for (int i = 0; i < n/2; ++i) {
            var1 += (sample1[i] - mean1) * (sample1[i] - mean1);
            var2 += (sample2[i] - mean2) * (sample2[i] - mean2);
        }
        var1 /= (n/2 - 1);
        var2 /= (n/2 - 1);

        // 计算t统计量（独立样本t检验）
        double pooled_var = ((n/2 - 1) * var1 + (n/2 - 1) * var2) / (n - 2);
        double se = sqrt(pooled_var * (2.0 / (n/2)));
        double t_statistic = (mean1 - mean2) / se;

        cout << "样本1均值: " << mean1 << ", 方差: " << var1 << endl;
        cout << "样本2均值: " << mean2 << ", 方差: " << var2 << endl;
        cout << "t统计量: " << t_statistic << endl;
        cout << "注意：需要查t分布表或使用统计库来确定p值" << endl << endl;
    }

    // 示例7: 汇总统计
    cout << "\n=== 7. 汇总统计 ===" << endl;
    {
        vector<double> sorted_data = data;
        sort(sorted_data.begin(), sorted_data.end());

        double min_val = sorted_data[0];
        double max_val = sorted_data[n-1];

        double mean = 0.0;
        for (int i = 0; i < n; ++i) {
            mean += data[i];
        }
        mean /= n;

        double variance = 0.0;
        for (int i = 0; i < n; ++i) {
            double diff = data[i] - mean;
            variance += diff * diff;
        }
        variance /= (n - 1);
        double stddev = sqrt(variance);

        double median;
        if (n % 2 == 0) {
            median = (sorted_data[n/2 - 1] + sorted_data[n/2]) / 2.0;
        } else {
            median = sorted_data[n/2];
        }

        cout << "数据汇总统计:" << endl;
        cout << "  最小值: " << min_val << endl;
        cout << "  最大值: " << max_val << endl;
        cout << "  均值: " << mean << endl;
        cout << "  中位数: " << median << endl;
        cout << "  标准差: " << stddev << endl;
        cout << "  方差: " << variance << endl;
        cout << "  范围: " << max_val - min_val << endl << endl;
    }

    cout << "\n注意：以上是统计函数的简化实现示例。" << endl;
    cout << "实际应用中，MKL提供了更高效的统计函数接口，" << endl;
    cout << "包括向量化的统计计算、批量处理等功能，" << endl;
    cout << "建议使用MKL的完整统计函数库以获得更好的性能。" << endl;

    cin.get();
    return 0;
}
