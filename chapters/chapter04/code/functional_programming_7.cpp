#include <iostream>
#include <functional>
// 函数组合器：compose(f, g) 返回一个新函数 h(x) = f(g(x))
template<typename F, typename G>
auto compose(F f, G g) {
    return [=](auto x) {
        return f(g(x));
    };
}
// 支持多函数组合（从右到左）
template<typename F, typename... Rest>
auto compose(F f, Rest... rest) {
    return [=](auto x) {
        return f(compose(rest...)(x));
    };
}

int main() {
    // 定义两个简单函数
    auto add2 = [](int x) { return x + 2; };
    auto mul3 = [](int x) { return x * 3; };
    // 组合：先加2再乘3
    auto add2_then_mul3 = compose(mul3, add2);
    std::cout << "add2_then_mul3(5) = " << add2_then_mul3(5) << std::endl; // (5+2)*3=21
    // 组合：先乘3再加2
    auto mul3_then_add2 = compose(add2, mul3);
    std::cout << "mul3_then_add2(5) = " << mul3_then_add2(5) << std::endl; // (5*3)+2=17
    // 多函数组合：((x+1)*2)-3
    auto add1 = [](int x) { return x + 1; };
    auto mul2 = [](int x) { return x * 2; };
    auto sub3 = [](int x) { return x - 3; };
    auto complex = compose(sub3, mul2, add1);
    std::cout << "complex(4) = " << complex(4) << std::endl; // ((4+1)*2)-3=7
    std::cin.get();
    return 0;
}
