#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// 高阶函数：接受函数作为参数，对vector中的每个元素应用该函数
void apply_to_each(std::vector<int>& v, const std::function<void(int&)>& func) {
    for (auto& x : v) { func(x); }
}

// 另一个高阶函数：接受函数作为参数，返回所有元素处理后的新vector
std::vector<int> map_vector(const std::vector<int>& v, const std::function<int(int)>& func) {
    std::vector<int> result;
    result.reserve(v.size());
    for (auto x : v) { result.push_back(func(x)); }
    return result;
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    // 1. 传递Lambda表达式作为参数，对每个元素加10
    apply_to_each(data, [](int& x) { x += 10; });
    std::cout << "每个元素加10后: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << std::endl;
    // 2. 使用map_vector，将每个元素平方，返回新vector
    auto squared = map_vector(data, [](int x) { return x * x; });
    std::cout << "每个元素平方后: ";
    for (auto x : squared) std::cout << x << " ";
    std::cout << std::endl;
    // 3. 也可以传递普通函数指针
    auto increment = [](int& x) { x++; };
    apply_to_each(data, increment);
    std::cout << "每个元素再加1后: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << std::endl;
    std::cin.get();
    return 0;
}
