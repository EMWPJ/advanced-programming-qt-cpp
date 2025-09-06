#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // if语句
    int x = 10;
    if (x > 5) {
        std::cout << "x大于5" << std::endl;
    }

    // for循环
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cin.get();
    return 0;
}