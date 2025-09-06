#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int a = 10, b = 3;

    // 算术运算符
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;

    // 关系运算符
    std::cout << "a > b: " << (a > b) << std::endl;

    // 逻辑运算符
    std::cout << "a && b: " << (a && b) << std::endl;

    std::cin.get();
    return 0;
}