#include <iostream>
#include <vector>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace boost::accumulators;
namespace acc = boost::accumulators;
using namespace boost::math;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost统计计算示例 ===" << endl;

    // 样本数据
    vector<double> data = {2.3, 2.1, 2.4, 2.2, 2.5, 2.0, 2.3, 2.6, 2.1, 2.4};

    // 使用Boost Accumulators计算基本统计量
    accumulator_set<double, stats<tag::mean, tag::variance, tag::skewness, tag::kurtosis,
                                  tag::min, tag::max, tag::median, tag::count>> acc;

    for (double value : data) {
        acc(value);
    }

    cout << "样本数据统计:" << endl;
    cout << "数据点数量: " << acc::count(acc) << endl;
    cout << "平均值: " << acc::mean(acc) << endl;
    cout << "方差: " << acc::variance(acc) << endl;
    cout << "标准差: " << sqrt(acc::variance(acc)) << endl;
    cout << "最小值: " << acc::min(acc) << endl;
    cout << "最大值: " << acc::max(acc) << endl;
    cout << "中位数: " << acc::median(acc) << endl;
    cout << "偏度: " << acc::skewness(acc) << endl;
    cout << "峰度: " << acc::kurtosis(acc) << endl;

    // 正态分布
    double mean_val = acc::mean(acc);
    double std_dev = sqrt(acc::variance(acc));

    normal_distribution<double> normal_dist(mean_val, std_dev);

    cout << "\n正态分布分析:" << endl;
    cout << "分布均值: " << boost::math::mean(normal_dist) << endl;
    cout << "分布标准差: " << standard_deviation(normal_dist) << endl;

    // 计算概率密度函数和累积分布函数
    double test_value = 2.3;
    cout << "在x = " << test_value << "处的:" << endl;
    cout << "  概率密度: " << pdf(normal_dist, test_value) << endl;
    cout << "  累积概率: " << cdf(normal_dist, test_value) << endl;
    cout << "  生存函数: " << 1.0 - cdf(normal_dist, test_value) << endl;

    // t分布 - 用于小样本假设检验
    int degrees_of_freedom = static_cast<int>(data.size()) - 1;
    students_t_distribution<double> t_dist(degrees_of_freedom);

    cout << "\nt分布分析 (自由度 = " << degrees_of_freedom << "):" << endl;
    double t_value = 2.0;
    cout << "t = " << t_value << " 的累积概率: " << cdf(t_dist, t_value) << endl;

    // 卡方分布 - 用于方差检验
    chi_squared_distribution<double> chi_dist(degrees_of_freedom);

    cout << "\n卡方分布分析 (自由度 = " << degrees_of_freedom << "):" << endl;
    double chi_value = 5.0;
    cout << "χ² = " << chi_value << " 的累积概率: " << cdf(chi_dist, chi_value) << endl;

    // 置信区间计算
    double confidence_level = 0.95;
    double alpha = 1.0 - confidence_level;
    double t_critical = boost::math::quantile(t_dist, 1.0 - alpha/2.0);

    double margin_of_error = t_critical * std_dev / sqrt(data.size());
    double ci_lower = mean_val - margin_of_error;
    double ci_upper = mean_val + margin_of_error;

    cout << "\n置信区间 (" << confidence_level*100 << "%):" << endl;
    cout << "[" << ci_lower << ", " << ci_upper << "]" << endl;

    cin.get();
    return 0;
}
