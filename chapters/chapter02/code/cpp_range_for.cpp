#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    for (auto num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cin.get();
    return 0;
}