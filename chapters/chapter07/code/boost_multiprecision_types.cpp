#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <iomanip>
#include <windows.h>

using namespace std;
using namespace boost::multiprecision;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost.Multiprecision类型选择示例 ===" << endl;

    // 1. 十进制浮点数类型
    cout << "\n1. 十进制浮点数类型:" << endl;

    typedef cpp_dec_float_50 dec_float_50;
    typedef cpp_dec_float_100 dec_float_100;

    dec_float_50 pi_50 = 3.14159265358979323846264338327950288419716939937510;
    dec_float_100 pi_100 = 3.14159265358979323846264338327950288419716939937510;

    cout << "50位精度 π: " << setprecision(50) << pi_50 << endl;
    cout << "100位精度 π: " << setprecision(100) << pi_100 << endl;

    // 2. 二进制浮点数类型（更快）
    cout << "\n2. 二进制浮点数类型:" << endl;

    typedef cpp_bin_float_50 bin_float_50;
    typedef cpp_bin_float_100 bin_float_100;

    bin_float_50 e_50 = 2.71828182845904523536028747135266249775724709369995;
    bin_float_100 e_100 = 2.71828182845904523536028747135266249775724709369995;

    cout << "50位二进制精度 e: " << setprecision(50) << e_50 << endl;
    cout << "100位二进制精度 e: " << setprecision(100) << e_100 << endl;

    // 3. 任意精度整数
    cout << "\n3. 任意精度整数:" << endl;

    cpp_int large_int = 1;
    for (int i = 1; i <= 50; ++i) {
        large_int *= i;  // 计算50!
    }

    cout << "50! = " << large_int << endl;
    cout << "50! 的位数: " << large_int.str().length() << endl;

    // 4. 有理数类型
    cout << "\n4. 有理数类型:" << endl;

    typedef cpp_rational rational_t;

    rational_t r1(22, 7);  // 22/7 (π的近似值)
    rational_t r2(355, 113);  // 355/113 (更好的π近似值)

    cout << "r1 = 22/7 = " << r1 << " ≈ " << (double)r1 << endl;
    cout << "r2 = 355/113 = " << r2 << " ≈ " << (double)r2 << endl;
    cout << "r1 + r2 = " << (r1 + r2) << endl;
    cout << "r1 * r2 = " << (r1 * r2) << endl;

    // 5. 类型转换和精度比较
    cout << "\n5. 精度比较:" << endl;

    dec_float_50 sqrt2_50 = sqrt(dec_float_50(2));
    dec_float_100 sqrt2_100 = sqrt(dec_float_100(2));

    cout << "√2 (50位): " << setprecision(50) << sqrt2_50 << endl;
    cout << "√2 (100位): " << setprecision(100) << sqrt2_100 << endl;

    // 6. 性能对比示例
    cout << "\n6. 性能提示:" << endl;
    cout << "十进制浮点 (cpp_dec_float): 精度高，但较慢" << endl;
    cout << "二进制浮点 (cpp_bin_float): 速度快，精度略低" << endl;
    cout << "固定精度类型: 比动态精度类型快" << endl;
    cout << "建议: 根据需求选择合适的精度和类型" << endl;

    cin.get();
    return 0;
}
