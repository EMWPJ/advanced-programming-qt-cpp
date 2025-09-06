#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int var = 10;
    int* ptr = &var;

    std::cout << "变量值: " << var << std::endl;
    std::cout << "指针值: " << *ptr << std::endl;

    *ptr = 20;
    std::cout << "修改后变量值: " << var << std::endl;

    std::cin.get();
    return 0;
}