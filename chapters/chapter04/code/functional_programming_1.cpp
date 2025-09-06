#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
int main() {
    // 1. 最基本的Lambda表达式
    auto add = [](int a, int b) { return a + b; };
    std::cout << "add(2, 3) = " << add(2, 3) << std::endl;
    // 2. Lambda作为std::function参数
    std::function<int(int, int)> mul = [](int a, int b) { return a * b; };
    std::cout << "mul(4, 5) = " << mul(4, 5) << std::endl;
    // 3. Lambda用于STL算法
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "原始数据: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << std::endl;
    // 使用Lambda对每个元素加1
    std::for_each(v.begin(), v.end(), [](int &x) { x += 1; });
    std::cout << "每个元素加1后: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << std::endl;
    // 4. 带值捕获的Lambda
    int factor = 10;
    auto multiply = [factor](int x) { return x * factor; };
    std::cout << "multiply(6) = " << multiply(6) << std::endl;
    // 5. Lambda作为排序谓词
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    std::cout << "降序排序后: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << std::endl;
    // 6. 引用捕获示例
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
    std::cout << "所有元素之和（引用捕获sum）: " << sum << std::endl;
    std::cin.get();
    return 0;
}
