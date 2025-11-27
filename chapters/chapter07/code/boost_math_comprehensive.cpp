#include <iostream>
#include <boost/math/special_functions.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/distributions/normal.hpp>
#include <iomanip>
#include <vector>
#include <windows.h>

using namespace std;
using namespace boost::math;
using namespace boost::math::constants;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << fixed << setprecision(6);
    cout << "=== Boost.Math综合应用示例 ===" << endl;

    // 1. 特殊函数计算
    cout << "\n1. 特殊函数计算:" << endl;
    double x = 2.5;
    cout << "贝塞尔函数 J_0(" << x << ") = " << cyl_bessel_j(0, x) << endl;
    cout << "误差函数 erf(" << x << ") = " << erf(x) << endl;
    cout << "伽马函数 Γ(" << x << ") = " << tgamma(x) << endl;

    // 2. 数学常数使用
    cout << "\n2. 数学常数:" << endl;
    cout << "π = " << pi<double>() << endl;
    cout << "e = " << e<double>() << endl;
    cout << "欧拉常数 γ = " << euler<double>() << endl;

    // 3. 概率分布函数
    cout << "\n3. 概率分布函数:" << endl;
    normal_distribution<double> normal(0.0, 1.0);  // 标准正态分布

    vector<double> test_points = {-2.0, -1.0, 0.0, 1.0, 2.0};
    cout << "标准正态分布 N(0,1):" << endl;
    cout << "x\t\tPDF(x)\t\tCDF(x)" << endl;
    cout << "----------------------------------------" << endl;
    for (double point : test_points) {
        cout << point << "\t\t" << pdf(normal, point)
             << "\t\t" << cdf(normal, point) << endl;
    }

    // 4. 分位数计算
    cout << "\n4. 分位数计算:" << endl;
    vector<double> probabilities = {0.05, 0.25, 0.5, 0.75, 0.95};
    cout << "概率\t\t分位数" << endl;
    cout << "----------------------------------------" << endl;
    for (double prob : probabilities) {
        double quantile_val = quantile(normal, prob);
        cout << prob << "\t\t" << quantile_val << endl;
    }

    // 5. 浮点数工具（使用 std:: 前缀避免命名冲突）
    cout << "\n5. 浮点数工具:" << endl;
    vector<double> test_values = {1.5, 0.0, -2.3,
                                   std::numeric_limits<double>::infinity()};
    for (double val : test_values) {
        cout << "值: " << val << " - ";
        cout << "isfinite: " << (std::isfinite(val) ? "是" : "否") << ", ";
        cout << "isnormal: " << (std::isnormal(val) ? "是" : "否") << endl;
    }

    // 6. 实际应用：计算正态分布的置信区间
    cout << "\n6. 实际应用：置信区间计算:" << endl;
    double mean_val = 100.0;
    double std_dev = 15.0;
    normal_distribution<double> dist(mean_val, std_dev);

    double confidence = 0.95;
    double alpha = 1.0 - confidence;
    double z_lower = quantile(dist, alpha / 2.0);
    double z_upper = quantile(dist, 1.0 - alpha / 2.0);

    cout << "均值: " << mean_val << ", 标准差: " << std_dev << endl;
    cout << confidence * 100 << "% 置信区间: ["
         << z_lower << ", " << z_upper << "]" << endl;

    cin.get();
    return 0;
}
