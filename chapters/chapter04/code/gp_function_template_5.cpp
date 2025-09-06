#include <iostream>
#include <concepts>
#include <string>

// 定义一个概念：要求类型T支持加法运算
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};
template<Addable T>
T add(T a, T b) {
    return a + b;
}

int main() {
    int x = 10, y = 20;
    std::cout << "int add: " << add(x, y) << std::endl;

    double d1 = 3.14, d2 = 2.71;
    std::cout << "double add: " << add(d1, d2) << std::endl;

    std::string s1 = "Hello, ", s2 = "World!";
    std::cout << "string add: " << add(s1, s2) << std::endl;

    // 下列代码将无法通过编译，因为指针不满足Addable概念
    // int* p1 = &x, *p2 = &y;
    // std::cout << "pointer add: " << add(p1, p2) << std::endl;

    std::cin.get();
    return 0;
}
