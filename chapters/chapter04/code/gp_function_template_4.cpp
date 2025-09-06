#include <iostream>

// 递归终止：只有一个参数时直接返回
template<typename T>
T TemplateSum(const T& value) {
    return value;
}

// 辅助函数，用于实现递归求和
template<typename T, typename... Args>
auto TemplateSum(const T& first, const Args&... args) {
    return first + TemplateSum(args...);
}

int main() {
    int a = 1, b = 2, c = 3;
    double d = 4.5, e = 5.5;
    std::cout << "TemplateSum(a, b, c) = " << TemplateSum(a, b, c) << std::endl;
    std::cout << "TemplateSum(a, b, c, d) = " << TemplateSum(a, b, c, d) << std::endl;
    std::cout << "TemplateSum(a, b, c, d, e) = " << TemplateSum(a, b, c, d, e) << std::endl;
    std::cin.get();
    return 0;
}
