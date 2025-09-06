#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    int count = 0;
    const double PI = 3.14159;

    std::cout << "计数: " << count << std::endl;
    std::cout << "PI: " << PI << std::endl;

    std::cin.get();
    return 0;
}