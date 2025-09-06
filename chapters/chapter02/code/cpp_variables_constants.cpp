#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int var = 10;
    const int CONST_VAR = 20;

    std::cout << "变量: " << var << std::endl;
    std::cout << "常量: " << CONST_VAR << std::endl;

    std::cin.get();
    return 0;
}