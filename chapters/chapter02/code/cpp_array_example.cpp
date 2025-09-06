#include <iostream>
#include <array>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // 原生数组示例
    int arr[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cin.get();
    return 0;
}