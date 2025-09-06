#include <iostream>
#include <string>
#include <windows.h>

// 函数声明
int add(int a, int b);
void printMessage(const std::string& message);
int factorial(int n);

int main() {
    SetConsoleOutputCP(65001);

    int result = add(5, 3);
    std::cout << "5 + 3 = " << result << std::endl;

    std::cin.get();
    return 0;
}

// 函数定义
int add(int a, int b) {
    return a + b;
}

// 引用参数示例
void printMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}

// 递归函数示例
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}