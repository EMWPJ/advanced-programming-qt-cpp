#include <iostream>

// 1. 使用constexpr递归计算阶乘
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// 2. 使用constexpr递归计算斐波那契数列
constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

// 3. 使用constexpr判断是否为2的幂
constexpr bool is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int main() {
    constexpr int fact5 = factorial(5);
    constexpr int fib8 = fibonacci(8);
    constexpr bool pow2_16 = is_power_of_two(16);
    constexpr bool pow2_18 = is_power_of_two(18);

    std::cout << "factorial(5) = " << fact5 << std::endl;
    std::cout << "fibonacci(8) = " << fib8 << std::endl;
    std::cout << "is_power_of_two(16) = " << std::boolalpha << pow2_16 << std::endl;
    std::cout << "is_power_of_two(18) = " << std::boolalpha << pow2_18 << std::endl;

    // constexpr结果可用于数组大小等
    int arr[factorial(4)] = {0};
    std::cout << "arr size = " << sizeof(arr) / sizeof(arr[0]) << std::endl;

    std::cin.get();
    return 0;
}
