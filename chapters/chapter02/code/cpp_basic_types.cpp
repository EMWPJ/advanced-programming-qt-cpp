#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // 基本类型示例
    int integer = 10;
    double floating = 3.14;
    char character = 'A';
    bool boolean = true;

    std::cout << "整数: " << integer << std::endl;
    std::cout << "浮点数: " << floating << std::endl;
    std::cout << "字符: " << character << std::endl;
    std::cout << "布尔值: " << std::boolalpha << boolean << std::endl;

    std::cin.get();
    return 0;
}