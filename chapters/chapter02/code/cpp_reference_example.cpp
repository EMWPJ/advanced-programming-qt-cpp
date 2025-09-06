#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int x = 10;
    int& ref = x;

    std::cout << "x = " << x << std::endl;
    std::cout << "ref = " << ref << std::endl;

    ref = 20;
    std::cout << "修改后 x = " << x << std::endl;

    std::cin.get();
    return 0;
}