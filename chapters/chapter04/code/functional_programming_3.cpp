#include <iostream>
#include <functional>

// 高阶函数：返回一个Lambda（函数）作为返回值
std::function<int(int)> make_multiplier(int factor) {
    // 返回一个捕获factor的Lambda
    return [factor](int x) {
        return x * factor;
    };
}

// 另一个示例：返回一个加法器
auto make_adder(int base) {
    return [base](int x) {
        return x + base;
    };
}

int main() {
    // 1. 生成一个乘以5的函数
    auto times5 = make_multiplier(5);
    std::cout << "times5(3) = " << times5(3) << std::endl; // 15

    // 2. 生成一个加10的函数
    auto add10 = make_adder(10);
    std::cout << "add10(7) = " << add10(7) << std::endl; // 17

    // 3. 组合使用
    std::cout << "times5(add10(2)) = " << times5(add10(2)) << std::endl; // 60

    std::cin.get();
    return 0;
}
