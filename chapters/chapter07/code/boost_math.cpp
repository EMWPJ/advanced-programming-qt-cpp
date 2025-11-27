#include <iostream>
#include <boost/math/special_functions.hpp>
#include <boost/math/special_functions/legendre.hpp>
#include <boost/math/constants/constants.hpp>
#include <vector>
#include <stdexcept>
#include <windows.h>

using namespace std;
using namespace boost::math;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost数学函数示例 ===" << endl;

    // 使用Boost常量
    cout << "数学常数:" << endl;
    cout << "π = " << constants::pi<double>() << endl;
    cout << "e = " << constants::e<double>() << endl;
    cout << "γ (Euler-Mascheroni) = " << constants::euler<double>() << endl;

    // 特殊函数
    double x = 1.5;
    cout << "\n特殊函数 (x = " << x << "):" << endl;

    // 贝塞尔函数
    cout << "J_0(" << x << ") = " << cyl_bessel_j(0, x) << endl;
    cout << "J_1(" << x << ") = " << cyl_bessel_j(1, x) << endl;

    // 误差函数
    cout << "erf(" << x << ") = " << erf(x) << endl;
    cout << "erfc(" << x << ") = " << erfc(x) << endl;

    // 伽马函数
    cout << "Γ(" << x << ") = " << tgamma(x) << endl;
    cout << "ln Γ(" << x << ") = " << lgamma(x) << endl;

    // 贝塔函数
    double a = 2.0, b = 3.0;
    cout << "B(" << a << ", " << b << ") = " << beta(a, b) << endl;

    // 椭圆积分
    double k = 0.5; // 模
    cout << "完整椭圆积分第一类 K(" << k << ") = " << ellint_1(k) << endl;
    cout << "完整椭圆积分第二类 E(" << k << ") = " << ellint_2(k) << endl;
    cout.flush();  // 确保输出被刷新

    // 勒让德多项式（注意：定义域为 [-1, 1]）
    int n = 3;
    double x_legendre = 0.5;  // 使用在有效范围内的值
    cout << "P_" << n << "(" << x_legendre << ") = ";
    cout.flush();
    try {
        // 使用更安全的调用方式，x必须在[-1, 1]范围内
        double legendre_val = boost::math::legendre_p(n, x_legendre);
        cout << legendre_val << endl;
    } catch (const std::exception& e) {
        cout << "计算错误: " << e.what() << endl;
    } catch (...) {
        cout << "计算时发生未知错误" << endl;
    }
    cout.flush();

    // 数学函数示例
    vector<double> values = {0.1, 0.5, 1.0, 2.0, 5.0};

    cout << "\n数学函数比较:" << endl;
    cout << "x\t\tsqrt(x)\t\tlog(x)\t\texp(x)" << endl;
    cout << "----------------------------------------" << endl;

    for (double val : values) {
        cout << val << "\t\t" << sqrt(val) << "\t\t";
        if (val > 0) {
            cout << log(val) << "\t\t";
        } else {
            cout << "N/A\t\t";
        }
        cout << exp(val) << endl;
    }

    cin.get();
    return 0;
}
