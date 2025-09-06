    // C++基本数据类型演示

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#define NOMINMAX
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);
    std::cout << "C++基本数据类型演示" << std::endl;
    std::cout << "==================" << std::endl;

    // 类型大小演示
    std::cout << "int大小: " << sizeof(int) << " 字节" << std::endl;
    std::cout << "double大小: " << sizeof(double) << " 字节" << std::endl;
    std::cout << "char大小: " << sizeof(char) << " 字节" << std::endl;

    // 类型转换
    int a = 10;
    double b = a; // 隐式转换
    std::cout << "int转double: " << b << std::endl;

    std::cin.get();
    return 0;
}