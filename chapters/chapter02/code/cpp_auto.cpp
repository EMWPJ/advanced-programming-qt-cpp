#include <iostream>
#include <vector>
#include <windows.h>

// 1. 使用迭代器遍历
void iteratorExample() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "使用迭代器遍历: ";
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 2. 使用范围for循环遍历
void rangeForExample() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "使用范围for循环遍历: ";
    for (auto number : numbers) {
        std::cout << number << " ";
    }
    std::cout << std::endl;
}

// 3. 使用模板打印向量
template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "打印向量: ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);

    // auto类型推导示例
    auto i = 5;          // int
    auto d = 3.14;       // double
    auto v = std::vector<int>{1, 2, 3};

    std::cout << "i type: " << typeid(i).name() << std::endl;
    std::cout << "d type: " << typeid(d).name() << std::endl;

    std::cin.get();
    return 0;
}