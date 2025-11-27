#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING
#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/constants/constants.hpp>
#include <windows.h>

using namespace std;
using namespace boost::multiprecision;
using namespace boost::math;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost高精度计算示例 ===" << endl;

    // 高精度浮点数
    typedef cpp_dec_float_50 mp_float_50;
    typedef cpp_dec_float_100 mp_float_100;

    // 计算圆周率的高精度值
    mp_float_50 pi_50 = constants::pi<mp_float_50>();
    mp_float_100 pi_100 = constants::pi<mp_float_100>();

    cout << "高精度圆周率计算:" << endl;
    cout << "50位精度: " << pi_50 << endl;
    cout << "100位精度: " << pi_100 << endl;

    // 高精度浮点数运算
    mp_float_50 a = mp_float_50("1.23456789012345678901234567890");
    mp_float_50 b = mp_float_50("9.87654321098765432109876543210");

    cout << "\n高精度浮点数运算:" << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    cout << "sqrt(a) = " << sqrt(a) << endl;

    // 高精度整数
    typedef cpp_int mp_int;

    mp_int large_num = 1;
    for (int i = 1; i <= 100; ++i) {
        large_num *= i;  // 计算100!
    }

    cout << "\n高精度整数计算:" << endl;
    cout << "100! = " << large_num << endl;
    cout << "100! 的位数: " << large_num.str().length() << endl;

    // 计算黄金分割比的高精度值
    mp_float_50 phi = (mp_float_50(1) + sqrt(mp_float_50(5))) / mp_float_50(2);
    cout << "\n黄金分割比 (50位精度): " << phi << endl;

    // 高精度计算e^x
    mp_float_50 x = mp_float_50("2.5");
    mp_float_50 exp_x = exp(x);
    cout << "exp(" << x << ") = " << exp_x << endl;

    // 验证精度
    mp_float_50 check = log(exp_x);
    cout << "log(exp(" << x << ")) = " << check << endl;
    cout << "误差: " << abs(check - x) << endl;

    // 高精度复数（使用 std::complex 避免命名冲突）
    typedef std::complex<mp_float_50> mp_complex;

    mp_complex z1(mp_float_50("1.5"), mp_float_50("2.3"));
    mp_complex z2(mp_float_50("0.7"), mp_float_50("-1.1"));

    cout << "\n高精度复数运算:" << endl;
    cout << "z1 = " << z1 << endl;
    cout << "z2 = " << z2 << endl;
    cout << "z1 + z2 = " << z1 + z2 << endl;
    cout << "z1 * z2 = " << z1 * z2 << endl;
    cout << "conj(z1) = " << conj(z1) << endl;
    cout << "|z1| = " << abs(z1) << endl;

    cin.get();
    return 0;
}
