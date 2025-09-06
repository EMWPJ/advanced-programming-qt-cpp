#include <iostream>
#include <type_traits>
// 1. 使用 if constexpr 实现类型分支
template<typename T>
void print_type_info(const T& value) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "类型为整型，值 = " << value << std::endl;
    } else if constexpr (std::is_floating_point<T>::value) {
        std::cout << "类型为浮点型，值 = " << value << std::endl;
    } else if constexpr (std::is_pointer<T>::value) {
        std::cout << "类型为指针，地址 = " << static_cast<const void*>(value) << std::endl;
    } else {
        std::cout << "未知类型" << std::endl;
    }
}
// 2. 使用 if constexpr 实现编译期递归
template<int N>
constexpr int factorial() {
    if constexpr (N <= 1) { return 1; }
    else { return N * factorial<N - 1>(); }
}
int main() {
    int a = 42;
    double d = 3.14;
    int* p = &a;
    print_type_info(a);   // 整型
    print_type_info(d);   // 浮点型
    print_type_info(p);   // 指针
    print_type_info("hello"); // 未知类型
    constexpr int fact5 = factorial<5>();
    std::cout << "factorial<5>() = " << fact5 << std::endl;
    std::cin.get();
    return 0;
}
