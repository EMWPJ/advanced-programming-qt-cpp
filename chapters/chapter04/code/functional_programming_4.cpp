#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
// 递归实现：阶乘
int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }
// 递归实现：斐波那契数列
int fibonacci(int n) { return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2); }
// 递归实现：vector求和
int sum_recursive(const std::vector<int>& v, size_t idx = 0) {
    if (idx >= v.size()) return 0;
    return v[idx] + sum_recursive(v, idx + 1);
}
// 组合式：map（对vector每个元素应用函数，返回新vector）
std::vector<int> map_vector(const std::vector<int>& v, const std::function<int(int)>& func) {
    std::vector<int> result;
    result.reserve(v.size());
    for (auto x : v) { result.push_back(func(x)); }
    return result;
}
// 组合式：filter（筛选满足条件的元素）
std::vector<int> filter_vector(const std::vector<int>& v, const std::function<bool(int)>& pred) {
    std::vector<int> result;
    for (auto x : v) { if (pred(x)) result.push_back(x); }
    return result;
}
// 组合式：reduce（归约/折叠）
int reduce_vector(const std::vector<int>& v, int init, const std::function<int(int, int)>& op) {
    int result = init;
    for (auto x : v) { result = op(result, x); }
    return result;
}

int main() {
    // 递归示例
    std::cout << "factorial(5) = " << factorial(5) << std::endl;
    std::cout << "fibonacci(8) = " << fibonacci(8) << std::endl;
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::cout << "sum_recursive = " << sum_recursive(data) << std::endl;
    // map: 所有元素平方
    auto squared = map_vector(data, [](int x) { return x * x; });
    std::cout << "map_vector平方: ";
    for (auto x : squared) std::cout << x << " ";
    std::cout << std::endl;
    // filter: 只保留偶数
    auto evens = filter_vector(data, [](int x) { return x % 2 == 0; });
    std::cout << "filter_vector偶数: ";
    for (auto x : evens) std::cout << x << " ";
    std::cout << std::endl;
    // reduce: 求和
    int sum = reduce_vector(data, 0, [](int a, int b) { return a + b; });
    std::cout << "reduce_vector求和: " << sum << std::endl;
    // 组合使用：先平方再求和
    int sum_of_squares = reduce_vector(
        map_vector(data, [](int x) { return x * x; }),
        0,
        [](int a, int b) { return a + b; }
    );
    std::cout << "先平方再求和: " << sum_of_squares << std::endl;
    std::cin.get();
    return 0;
}
