#include <iostream>
#include <type_traits>
// 1. 使用 std::integral_constant 定义编译期常量
using Five = std::integral_constant<int, 5>;
using TrueType = std::integral_constant<bool, true>;
// 2. 编译期计算阶乘（递归模板，继承自 std::integral_constant）
template<int N>
struct Factorial : std::integral_constant<int, N * Factorial<N - 1>::value> {};
template<>
struct Factorial<0> : std::integral_constant<int, 1> {};
// 3. 编译期判断是否为偶数
template<int N>
struct IsEven : std::integral_constant<bool, (N % 2 == 0)> {};
int main() {
    // std::integral_constant 的基本用法
    std::cout << "Five::value = " << Five::value << std::endl;
    std::cout << "TrueType::value = " << std::boolalpha << TrueType::value << std::endl;
    // Factorial
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Factorial<0>::value = " << Factorial<0>::value << std::endl;
    // IsEven
    std::cout << "IsEven<4>::value = " << std::boolalpha << IsEven<4>::value << std::endl;
    std::cout << "IsEven<7>::value = " << std::boolalpha << IsEven<7>::value << std::endl;
    // std::integral_constant 可用于类型萃取
    if constexpr (IsEven<10>::value) {
        std::cout << "10 是偶数" << std::endl;
    } else {
        std::cout << "10 是奇数" << std::endl;
    }
    std::cin.get();
    return 0;
}
