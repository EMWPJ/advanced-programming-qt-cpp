// C++ Basic Syntax Example

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

// 基本数据类型
int integer = 42;
short short_int = 16;
long long_int = 32;
long long long_long_int = 64;
unsigned int unsigned_int = 100;
char character = 'A';
float float_num = 3.14f;
double double_num = 3.14159;
long double long_double_num = 3.14159265359L;
bool is_boolean = true;
wchar_t wide_char = L'W';

int main() {
    SetConsoleOutputCP(65001);

    // 基本语法示例
    std::cout << "Hello, World!" << std::endl;

    int a = 5;
    int b = 3;
    std::cout << "a + b = " << a + b << std::endl;

    std::cin.get();
    return 0;
}