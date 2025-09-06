#include <array>
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    for (auto num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cin.get();
    return 0;
}