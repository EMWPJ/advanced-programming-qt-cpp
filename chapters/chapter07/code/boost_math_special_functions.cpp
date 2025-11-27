#include <iostream>
#include <boost/math/special_functions.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/constants/constants.hpp>
#include <vector>
#include <iomanip>
#include <windows.h>

using namespace std;
using namespace boost::math;
using namespace boost::math::constants;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << fixed << setprecision(6);
    cout << "=== Boost.Math特殊函数示例 ===" << endl;

    double x = 2.5;

    // 贝塞尔函数
    cout << "\n1. 贝塞尔函数:" << endl;
    cout << "J_0(" << x << ") = " << cyl_bessel_j(0, x) << endl;
    cout << "J_1(" << x << ") = " << cyl_bessel_j(1, x) << endl;
    // 注意：cyl_bessel_y 在某些 Boost 版本中可能不可用，使用 cyl_neumann 代替
    double y0_val = cyl_neumann(0, x);
    double y1_val = cyl_neumann(1, x);
    cout << "Y_0(" << x << ") = " << y0_val << endl;
    cout << "Y_1(" << x << ") = " << y1_val << endl;

    // 椭圆积分
    cout << "\n2. 椭圆积分:" << endl;
    double k = 0.5; // 模
    cout << "K(" << k << ") = " << ellint_1(k) << " (第一类完整椭圆积分)" << endl;
    cout << "E(" << k << ") = " << ellint_2(k) << " (第二类完整椭圆积分)" << endl;

    // 伽马函数
    cout << "\n3. 伽马函数:" << endl;
    cout << "Γ(" << x << ") = " << tgamma(x) << endl;
    cout << "ln Γ(" << x << ") = " << lgamma(x) << endl;
    double a = 2.0, b = 3.0;
    cout << "B(" << a << ", " << b << ") = " << beta(a, b) << " (贝塔函数)" << endl;

    // 误差函数
    cout << "\n4. 误差函数:" << endl;
    cout << "erf(" << x << ") = " << erf(x) << endl;
    cout << "erfc(" << x << ") = " << erfc(x) << endl;
    double erf_val = erf(x);
    cout << "erf_inv(" << erf_val << ") = " << erf_inv(erf_val) << endl;

    // 勒让德多项式（注意：定义域为 [-1, 1]）
    cout << "\n5. 勒让德多项式:" << endl;
    double x_legendre = 0.5;  // 使用在有效范围内的值
    for (int n = 0; n <= 4; ++n) {
        cout << "P_" << n << "(" << x_legendre << ") = " << legendre_p(n, x_legendre) << endl;
    }

    // 数学常数
    cout << "\n6. 数学常数:" << endl;
    cout << "π = " << pi<double>() << endl;
    cout << "e = " << e<double>() << endl;
    cout << "γ (Euler-Mascheroni) = " << euler<double>() << endl;
    cout << "φ (Golden Ratio) = " << phi<double>() << endl;

    // 浮点数工具（使用 std:: 前缀避免命名冲突）
    cout << "\n7. 浮点数工具:" << endl;
    vector<double> test_values = {1.5, 0.0, -2.3, std::numeric_limits<double>::infinity(),
                                   std::numeric_limits<double>::quiet_NaN()};
    for (double val : test_values) {
        cout << "值: " << val << " - ";
        cout << "isfinite: " << (std::isfinite(val) ? "是" : "否") << ", ";
        cout << "isnormal: " << (std::isnormal(val) ? "是" : "否") << ", ";
        cout << "signbit: " << (std::signbit(val) ? "负" : "正/零") << endl;
    }

    cin.get();
    return 0;
}
