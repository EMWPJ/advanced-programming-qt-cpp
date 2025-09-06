#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);
    std::vector<int> vec = {1, 2, 3, 4, 5}; // 初始化
    for (auto x : vec) {
        std::cout << x << " " << std::endl;
    }
    std::cout << "vec.size() = " << vec.size() << std::endl;
    std::cout << "vec.front() = " << vec.front() << std::endl;
    std::cout << "vec.back() = " << vec.back() << std::endl;
    std::cout << "vec.at(2) = " << vec.at(2) << std::endl;
    std::cout << "vec[2] = " << vec[2] << std::endl;
    std::cout << "vec.empty() = " << vec.empty() << std::endl;
    //迭代器
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " " << std::endl;
    }
    //插入元素
    vec.insert(vec.begin() + 2, 10);
    for (auto x : vec) {
        std::cout << x << " " << std::endl;
    }
    //删除元素
    vec.erase(vec.begin() + 2);
    for (auto x : vec) {
        std::cout << x << " " << std::endl;
    }
    //清空元素
    vec.clear();
    for (auto x : vec) {
        std::cout << x << " " << std::endl;
    }
    std::cin.get();
    return 0;
}
