#include <iostream>
#include <vector>
#include <chrono>
#include <random>

// 过程式编程：性能关键代码示例——大规模数组求和
int main() {
    const size_t N = 100000000; // 1亿个元素
    std::vector<int> data(N);

    // 随机填充数据
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < N; ++i) {
        data[i] = dist(rng);
    }

    // 性能计时开始
    auto start = std::chrono::high_resolution_clock::now();

    // 求和
    long long sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += data[i];
    }

    // 性能计时结束
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "数据总和: " << sum << std::endl;
    std::cout << "耗时: " << elapsed.count() << " 秒" << std::endl;

    std::cin.get();
    return 0;
}
