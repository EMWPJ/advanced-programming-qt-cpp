#include <iostream>
#include <string>
#include <windows.h>

// 重载函数：不同参数类型
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

// 重载函数：不同参数数量
int add(int a, int b, int c) {
    return a + b + c;
}

// 重载函数：不同参数类型
std::string add(const std::string& a, const std::string& b) {
    return a + b;
}

void print(int i) { std::cout << "整数: " << i << std::endl; }
void print(double f) { std::cout << "浮点数: " << f << std::endl; }
void print(const char* s) { std::cout << "字符串: " << s << std::endl; }

int main() {
    SetConsoleOutputCP(65001);

    print(10);
    print(3.14);
    print("Hello");

    std::cin.get();
    return 0;
}