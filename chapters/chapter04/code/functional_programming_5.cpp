#include <iostream>
#include <vector>
#include <algorithm>
// 定义一个函数对象（仿函数）：加法器
struct Adder {
    int operator()(int a, int b) const { return a + b; }
};
// 定义一个函数对象：判断是否为偶数
struct IsEven {
    bool operator()(int x) const { return x % 2 == 0; }
};
// 定义一个函数对象：对vector中每个元素加上指定值
struct AddN {
    int n;
    AddN(int n_) : n(n_) {}
    void operator()(int& x) const { x += n; }
};

int main() {
    // 1. 函数对象作为可调用对象
    Adder add;
    std::cout << "Adder(3, 4) = " << add(3, 4) << std::endl;
    // 2. 函数对象用于STL算法
    std::vector<int> v = {1, 2, 3, 4, 5};
    // 使用AddN对每个元素加10
    std::for_each(v.begin(), v.end(), AddN(10));
    std::cout << "每个元素加10后: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << std::endl;
    // 使用IsEven筛选偶数
    std::cout << "偶数元素: ";
    std::for_each(v.begin(), v.end(), [](int x){
        if (IsEven{}(x)) std::cout << x << " ";
    });
    std::cout << std::endl;
    std::cin.get();
    return 0;
}
