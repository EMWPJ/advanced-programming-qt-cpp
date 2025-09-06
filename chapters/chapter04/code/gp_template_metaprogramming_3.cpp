#include <iostream>
// 1. 编译期计算阶乘（递归模板）
template<int N>
struct Factorial {
    static_assert(N >= 0, "N 必须为非负数");
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<> struct Factorial<0> { static constexpr int value = 1; };
// 2. 编译期计算斐波那契数列（递归模板）
template<int N>
struct Fibonacci {
    static_assert(N >= 0, "N 必须为非负数");
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};
template<> struct Fibonacci<1> { static constexpr int value = 1; };
template<> struct Fibonacci<0> { static constexpr int value = 0; };

// 3. 编译期判断是否为2的幂
template<int N>
struct IsPowerOfTwo {
    static_assert(N > 0, "N 必须为正数");
    static constexpr bool value = (N & (N - 1)) == 0;
};
int main() {
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Fibonacci<8>::value = " << Fibonacci<8>::value << std::endl;
    std::cout << "IsPowerOfTwo<16>::value = " << std::boolalpha << IsPowerOfTwo<16>::value << std::endl;
    std::cout << "IsPowerOfTwo<18>::value = " << std::boolalpha << IsPowerOfTwo<18>::value << std::endl;
    // 编译期常量可用于数组大小等
    int arr[Factorial<4>::value] = {0};
    std::cout << "arr size = " << sizeof(arr) / sizeof(arr[0]) << std::endl;
    return 0;
}
