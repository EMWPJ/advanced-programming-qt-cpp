#include <iostream>
#include <cmath>
#include <utility>
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/constants/constants.hpp>
#include <windows.h>

using namespace std;
using namespace boost::math;
using namespace boost::math::quadrature;
using namespace boost::math::tools;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost.Math高级应用示例 ===" << endl;

    // 示例1: 数值积分 - Gauss-Kronrod方法
    cout << "\n1. 数值积分示例:" << endl;

    // 定义被积函数: sin(x) * exp(-x)
    auto integrand = [](double x) {
        return sin(x) * exp(-x);
    };

    // 使用Gauss-Kronrod方法进行积分
    double a = 0.0;
    double b = 1.0;
    double tolerance_integrate = 1e-6;
    unsigned int max_refinements = 15U;

    // gauss_kronrod::integrate 的参数顺序：函数、下限、上限、最大细化级别、容差
    double result = gauss_kronrod<double, 15>::integrate(
        integrand, a, b, max_refinements, tolerance_integrate);

    cout << "积分区间: [" << a << ", " << b << "]" << endl;
    cout << "被积函数: sin(x) * exp(-x)" << endl;
    cout << "积分结果: " << result << endl;

    // 示例2: 计算定积分 ∫[0,π] sin(x) dx = 2
    auto sin_func = [](double x) {
        return sin(x);
    };

    double pi_val = constants::pi<double>();
    double sin_integral = gauss_kronrod<double, 15>::integrate(
        sin_func, 0.0, pi_val);

    cout << "\n定积分 ∫[0,π] sin(x) dx:" << endl;
    cout << "计算结果: " << sin_integral << endl;
    cout << "理论值: 2.0" << endl;
    cout << "误差: " << abs(sin_integral - 2.0) << endl;

    // 示例3: 根查找 - 二分法
    cout << "\n2. 根查找示例:" << endl;

    // 定义函数: f(x) = x² - 2
    auto f = [](double x) {
        return x * x - 2.0;
    };

    // 使用二分法查找根（√2）
    // bisect 函数需要一个接受单个参数并返回函数值的函数对象
    double low = 1.0;
    double high = 2.0;
    auto tolerance = eps_tolerance<double>();

    auto result_pair = bisect(f, low, high, tolerance);
    double root = (result_pair.first + result_pair.second) / 2.0;

    cout << "函数: f(x) = x² - 2" << endl;
    cout << "搜索区间: [" << low << ", " << high << "]" << endl;
    cout << "找到的根: " << root << endl;
    cout << "理论值: √2 ≈ " << sqrt(2.0) << endl;
    cout << "函数值: f(" << root << ") = " << f(root) << endl;

    // 示例4: 使用Newton-Raphson方法
    cout << "\n3. Newton-Raphson方法:" << endl;

    // 定义函数和导数: f(x) = x³ - x - 2
    auto f2 = [](double x) {
        return x * x * x - x - 2.0;
    };

    auto df2 = [](double x) {
        return 3.0 * x * x - 1.0;
    };

    // 使用Newton-Raphson方法
    // 注意：newton_raphson_iterate 需要函数对象，包含函数值和导数值
    double guess = 1.5;
    double min = 1.0;
    double max = 2.0;
    int digits = std::numeric_limits<double>::digits10;

    // 创建一个函数对象，包含函数值和导数
    auto f2_with_derivative = [&f2, &df2](double x) -> std::pair<double, double> {
        return std::make_pair(f2(x), df2(x));
    };

    double result_newton = newton_raphson_iterate(
        f2_with_derivative, guess, min, max, digits);

    cout << "函数: f(x) = x³ - x - 2" << endl;
    cout << "初始猜测: " << guess << endl;
    cout << "找到的根: " << result_newton << endl;
    cout << "函数值: f(" << result_newton << ") = "
         << f2(result_newton) << endl;

    // 示例5: 数值积分 - 更复杂的函数
    cout << "\n4. 复杂函数积分:" << endl;

    // 积分: ∫[0,1] exp(-x²) dx (误差函数相关)
    auto exp_square = [](double x) {
        return exp(-x * x);
    };

    double erf_integral = gauss_kronrod<double, 15>::integrate(
        exp_square, 0.0, 1.0);

    cout << "积分: ∫[0,1] exp(-x²) dx" << endl;
    cout << "计算结果: " << erf_integral << endl;
    cout << "理论值: (√π/2) * erf(1) ≈ "
         << (sqrt(pi_val) / 2.0) * erf(1.0) << endl;

    // 示例6: 多重根查找
    cout << "\n5. 查找多个根:" << endl;

    // 函数: f(x) = sin(x) - x/2
    auto f3 = [](double x) {
        return sin(x) - x / 2.0;
    };

    // 在区间[0, 5]中查找根
    vector<double> roots;
    auto tolerance_root = eps_tolerance<double>();
    for (double start = 0.0; start < 5.0; start += 0.5) {
        double end = start + 0.5;
        if (f3(start) * f3(end) < 0) {
            auto root_pair = bisect(f3, start, end, tolerance_root);
            double root_val = (root_pair.first + root_pair.second) / 2.0;
            roots.push_back(root_val);
        }
    }

    cout << "函数: f(x) = sin(x) - x/2" << endl;
    cout << "在区间[0, 5]中找到的根:" << endl;
    for (size_t i = 0; i < roots.size(); ++i) {
        cout << "  根 " << (i+1) << ": " << roots[i]
             << " (f(x) = " << f3(roots[i]) << ")" << endl;
    }

    cin.get();
    return 0;
}
