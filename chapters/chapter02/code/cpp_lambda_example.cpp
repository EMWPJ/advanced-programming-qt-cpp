#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Lambda表达式
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cin.get();
    return 0;
}