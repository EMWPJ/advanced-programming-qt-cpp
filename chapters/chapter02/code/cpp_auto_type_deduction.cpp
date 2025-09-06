#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <type_traits>
#include <windows.h>

const int& getValue() {
    static int x = 10; // 使x静态以避免返回局部引用
    return x;
}

int main() {
    SetConsoleOutputCP(65001);

    // auto类型推导示例
    auto x = 5;          // int
    auto y = 3.14;       // double
    auto z = "hello";    // const char*

    std::cout << "x type: " << typeid(x).name() << std::endl;
    std::cout << "y type: " << typeid(y).name() << std::endl;
    std::cout << "z type: " << typeid(z).name() << std::endl;

    std::cin.get();
    return 0;
}