#include <iostream>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <iomanip>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace boost::accumulators;
namespace acc = boost::accumulators;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << fixed << setprecision(4);
    cout << "=== Boost.Accumulators统计量示例 ===" << endl;

    // 示例数据
    vector<double> data = {2.3, 2.1, 2.4, 2.2, 2.5, 2.0, 2.3, 2.6, 2.1, 2.4,
                           2.7, 2.2, 2.5, 2.3, 2.4, 2.1, 2.6, 2.3, 2.4, 2.2};

    // 1. 基本统计量
    cout << "\n1. 基本统计量:" << endl;
    accumulator_set<double, stats<tag::mean, tag::variance, tag::min,
                                  tag::max, tag::count, tag::sum>> acc_basic;

    for (double value : data) {
        acc_basic(value);
    }

    cout << "数据点数量: " << acc::count(acc_basic) << endl;
    cout << "总和: " << acc::sum(acc_basic) << endl;
    cout << "均值: " << acc::mean(acc_basic) << endl;
    cout << "方差: " << acc::variance(acc_basic) << endl;
    cout << "标准差: " << sqrt(acc::variance(acc_basic)) << endl;
    cout << "最小值: " << acc::min(acc_basic) << endl;
    cout << "最大值: " << acc::max(acc_basic) << endl;

    // 2. 中位数和分位数
    cout << "\n2. 中位数和分位数:" << endl;
    accumulator_set<double, stats<tag::median, tag::p_square_quantile>> acc_median;

    for (double value : data) {
        acc_median(value);
    }

    cout << "中位数: " << acc::median(acc_median) << endl;

    // 3. 高级统计量：偏度和峰度
    cout << "\n3. 高级统计量:" << endl;
    accumulator_set<double, stats<tag::mean, tag::variance, tag::skewness,
                                  tag::kurtosis>> acc_advanced;

    for (double value : data) {
        acc_advanced(value);
    }

    cout << "偏度: " << acc::skewness(acc_advanced) << " (分布不对称性)" << endl;
    cout << "峰度: " << acc::kurtosis(acc_advanced) << " (分布尖锐度)" << endl;

    // 4. 统计矩
    cout << "\n4. 统计矩:" << endl;
    accumulator_set<double, stats<tag::mean, tag::moment<2>,
                                  tag::moment<3>, tag::moment<4>>> acc_moments;

    for (double value : data) {
        acc_moments(value);
    }

    cout << "一阶矩 (均值): " << acc::mean(acc_moments) << endl;
    cout << "二阶矩: " << acc::moment<2>(acc_moments) << endl;
    cout << "三阶矩: " << acc::moment<3>(acc_moments) << endl;
    cout << "四阶矩: " << acc::moment<4>(acc_moments) << endl;

    // 5. 加权统计量
    cout << "\n5. 加权统计量:" << endl;
    accumulator_set<double, stats<tag::weighted_mean>,
                    double> acc_weighted(weight = 1.0);

    // 给不同数据点不同的权重
    vector<double> weights = {1.0, 1.5, 1.0, 1.2, 1.0, 0.8, 1.0, 1.3, 1.0, 1.1,
                              1.0, 1.2, 1.0, 1.0, 1.1, 0.9, 1.0, 1.2, 1.0, 1.0};

    for (size_t i = 0; i < data.size(); ++i) {
        acc_weighted(data[i], weight = weights[i]);
    }

    cout << "加权均值: " << acc::weighted_mean(acc_weighted) << endl;
    cout << "普通均值: " << acc::mean(acc_basic) << endl;

    // 6. 流式处理示例
    cout << "\n6. 流式处理示例:" << endl;
    accumulator_set<double, stats<tag::mean, tag::variance, tag::count>> acc_stream;

    cout << "逐个添加数据点并查看统计量变化:" << endl;
    size_t max_count = (5UL < data.size()) ? 5UL : data.size();
    for (size_t i = 0; i < max_count; ++i) {
        acc_stream(data[i]);
        cout << "添加 " << data[i] << " -> ";
        cout << "均值: " << acc::mean(acc_stream) << ", ";
        cout << "方差: " << acc::variance(acc_stream) << ", ";
        cout << "计数: " << acc::count(acc_stream) << endl;
    }

    cin.get();
    return 0;
}
