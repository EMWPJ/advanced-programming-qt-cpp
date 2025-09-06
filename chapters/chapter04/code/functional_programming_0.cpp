#include <iostream>

// 纯函数：只依赖输入参数，无副作用
int add(int a, int b) {
    return a + b;
}

// 纯函数：计算平方
int square(int x) {
    return x * x;
}

// 纯函数：返回较大值
int max_value(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int x = 3, y = 5;
    std::cout << "add(3, 5) = " << add(x, y) << std::endl;
    std::cout << "square(4) = " << square(4) << std::endl;
    std::cout << "max_value(7, 2) = " << max_value(7, 2) << std::endl;
    std::cin.get();
    return 0;
}
