#include <iostream>
#include <type_traits>
// 1. 静态断言：编译期检查类型是否为整型
template<typename T>
void check_integral() {
    static_assert(std::is_integral<T>::value, "T 必须是整型类型");
}
// 2. 静态断言：编译期检查数组大小
template<typename T, std::size_t N>
void check_array_size(T (&)[N]) {
    static_assert(N > 3, "数组大小必须大于3");
}
// 3. 静态断言：自定义条件
template<int N>
struct Factorial {
    static_assert(N >= 0, "N 必须为非负数");
    static constexpr int value = N * Factorial<N - 1>::value;
};
template<> struct Factorial<0> { static constexpr int value = 1; };

int main() {
    // 检查类型
    check_integral<int>();
    // check_integral<double>(); // 取消注释将导致编译错误

    // 检查数组大小
    int arr[5] = {};
    check_array_size(arr);
    // int arr2[2] = {};
    // check_array_size(arr2); // 取消注释将导致编译错误

    // 编译期计算阶乘
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    // Factorial<-1> f; // 取消注释将导致编译错误

    return 0;
}
